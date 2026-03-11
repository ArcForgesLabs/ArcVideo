#!/usr/bin/env python3
"""Generate a Visual Studio solution for the ArcVideo project.

Automates the two-step workflow:
  1. cmake --preset <preset>  → generates the VS solution (.slnx / .sln)
  2. cmake --build            → builds only third-party libraries (OpenTimelineIO)
     for each requested configuration so the solution opens cleanly.

After running this script, open the .slnx in Visual Studio and develop
normally — only the arcvideo-editor targets need manual building.

Usage:
    python tools/GenerateSln.py                 # default: windows-vs, Debug+Release
    python tools/GenerateSln.py --open          # open VS after generation
    python tools/GenerateSln.py --skip-configure
    python tools/GenerateSln.py --configs Debug  # single config
"""

from __future__ import annotations

import argparse
import json
import os
import platform
import subprocess
import sys
import time
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent

# ── Pretty helpers ───────────────────────────────────────────────

RESET = "\033[0m"
BOLD = "\033[1m"
DIM = "\033[2m"
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
WHITE = "\033[97m"


def _supports_color() -> bool:
    if os.environ.get("NO_COLOR"):
        return False
    if sys.platform == "win32":
        return os.environ.get("TERM") or os.environ.get("WT_SESSION")
    return hasattr(sys.stdout, "isatty") and sys.stdout.isatty()


USE_COLOR = _supports_color()


def c(color: str, text: str) -> str:
    return f"{color}{text}{RESET}" if USE_COLOR else text


def log(msg: str, color: str = WHITE) -> None:
    ts = time.strftime("%H:%M:%S")
    print(f"{c(DIM, f'[{ts}]')} {c(color, msg)}")


def banner() -> None:
    print(c(CYAN, "╔══════════════════════════════════════════╗"))
    print(c(CYAN, "║        GenerateSln v1.0.0                ║"))
    print(c(CYAN, "╚══════════════════════════════════════════╝"))


# ── Project root detection ───────────────────────────────────────


def find_project_root(explicit: str | None = None) -> Path:
    if explicit:
        p = Path(explicit)
        if (p / "CMakeLists.txt").is_file():
            return p.resolve()

    # Git root
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--show-toplevel"],
            capture_output=True,
            text=True,
            check=True,
        )
        git_root = Path(result.stdout.strip())
        if (git_root / "CMakeLists.txt").is_file():
            return git_root.resolve()
    except (subprocess.CalledProcessError, FileNotFoundError):
        # Git unavailable or not a repo — fall back to other detection strategies
        pass

    # Script parent
    if (PROJECT_ROOT / "CMakeLists.txt").is_file():
        return PROJECT_ROOT

    # CWD
    cwd = Path.cwd()
    if (cwd / "CMakeLists.txt").is_file():
        return cwd.resolve()

    print(c(RED, "Error: No CMakeLists.txt found."))
    print("  Run from a CMake project root, a subdirectory (e.g. tools/),")
    print("  or pass --project-root.")
    sys.exit(1)


# ── Build directory resolution ───────────────────────────────────


def get_build_dir(root: Path, preset_name: str) -> Path:
    presets_file = root / "CMakePresets.json"
    if presets_file.is_file():
        with open(presets_file) as f:
            presets = json.load(f)
        for p in presets.get("configurePresets", []):
            if p.get("name") == preset_name and "binaryDir" in p:
                d = p["binaryDir"].replace("${sourceDir}", str(root))
                return Path(d)
    # Fallback
    config = preset_name.removeprefix("windows-")
    return root / "build" / config


# ── Main ─────────────────────────────────────────────────────────

VALID_CONFIGS = {"Debug", "Release", "RelWithDebInfo", "MinSizeRel"}
THIRD_PARTY_TARGETS = ["opentimelineio", "opentime"]


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generate a Visual Studio solution for ArcVideo."
    )
    parser.add_argument(
        "--project-root",
        default=None,
        help="Path to CMake project root (auto-detected)",
    )
    parser.add_argument(
        "--preset",
        default="windows-vs",
        help="CMake configure preset (default: windows-vs)",
    )
    parser.add_argument(
        "--configs",
        nargs="+",
        default=["Debug", "Release"],
        help="Build configurations (default: Debug Release)",
    )
    parser.add_argument(
        "--skip-configure", action="store_true", help="Skip cmake configure step"
    )
    parser.add_argument(
        "--open",
        action="store_true",
        help="Open the solution in Visual Studio after generation",
    )
    args = parser.parse_args()

    for cfg in args.configs:
        if cfg not in VALID_CONFIGS:
            print(
                c(
                    RED,
                    f"Error: Invalid config '{cfg}'. Choose from: {', '.join(sorted(VALID_CONFIGS))}",
                )
            )
            sys.exit(1)

    root = find_project_root(args.project_root)
    build_dir = get_build_dir(root, args.preset)
    config_list = ", ".join(args.configs)

    banner()
    print(f"  Project : {root}")
    print(f"  Preset  : {args.preset}")
    print(f"  Configs : {config_list}")
    print(f"  BuildDir: {build_dir}")
    print()

    start = time.monotonic()

    # Step 1: Configure
    if not args.skip_configure:
        log(f">> Step 1: cmake --preset {args.preset}", CYAN)
        result = subprocess.run(
            ["cmake", "--preset", args.preset],
            cwd=root,
        )
        if result.returncode != 0:
            log("✗ cmake configure failed!", RED)
            sys.exit(1)
        log("✓ Configure succeeded", GREEN)
    else:
        log("-- Skipping configure", YELLOW)
        if not build_dir.is_dir():
            print(c(RED, f"Error: Build directory not found: {build_dir}"))
            print("  Run without --skip-configure first.")
            sys.exit(1)

    print()

    # Step 2: Build third-party libraries
    log(">> Step 2: Building third-party libraries...", CYAN)
    log(f"  Targets: {', '.join(THIRD_PARTY_TARGETS)}", WHITE)

    for cfg in args.configs:
        log(f">> Building Configuration: [{cfg}]", YELLOW)
        for target in THIRD_PARTY_TARGETS:
            log(f"  Building {target} ({cfg})...", WHITE)
            build_cmd = [
                "cmake",
                "--build",
                str(build_dir),
                "--config",
                cfg,
                "--target",
                target,
            ]
            if platform.system() == "Windows":
                build_cmd += ["--", "/v:m", "/nologo"]
            result = subprocess.run(build_cmd)
            if result.returncode != 0:
                log(f"✗ Failed to build {target} for {cfg}!", RED)
                sys.exit(1)
            log(f"  ✓ {target} ({cfg}) built", GREEN)

    elapsed = time.monotonic() - start
    print()
    log(f"✓ All done in {elapsed:.1f}s", GREEN)

    # Find .slnx / .sln
    sln_file = None
    for pattern in ("*.slnx", "*.sln"):
        matches = list(build_dir.glob(pattern))
        if matches:
            sln_file = matches[0]
            break

    if sln_file:
        print()
        log(f"Solution: {sln_file}", CYAN)
        if args.open:
            log("Opening in Visual Studio...", CYAN)
            if platform.system() == "Windows":
                os.startfile(str(sln_file))  # type: ignore[attr-defined]
            elif platform.system() == "Darwin":
                subprocess.run(["open", str(sln_file)])
            else:
                subprocess.run(["xdg-open", str(sln_file)])
        else:
            log("Run with --open to launch Visual Studio, or open manually:", WHITE)
            print(f"    {sln_file}")


if __name__ == "__main__":
    main()
