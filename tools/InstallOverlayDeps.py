#!/usr/bin/env python3
"""Install or uninstall ArcVideoRegistry overlay-port packages via vcpkg.

Dynamically discovers all ports from ArcVideoRegistry, detects which are
already installed locally, and presents an interactive menu to install or
uninstall them.

Usage:
    python tools/InstallOverlayDeps.py              # interactive install
    python tools/InstallOverlayDeps.py --all         # install all without prompting
    python tools/InstallOverlayDeps.py --uninstall   # interactive uninstall
"""

from __future__ import annotations

import argparse
import json
import os
import platform
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

REGISTRY_URL = "https://github.com/ArcForgesLabs/ArcVideoRegistry.git"
SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent
REGISTRY_DIR = PROJECT_ROOT / "build" / "vcpkg_registry"

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
    print(c(CYAN, "║      InstallOverlayDeps v2.0.0           ║"))
    print(c(CYAN, "╚══════════════════════════════════════════╝"))


# ── System detection ─────────────────────────────────────────────


def detect_system() -> str:
    s = platform.system()
    if s == "Windows":
        return "Windows"
    if s == "Linux":
        return "Linux"
    if s == "Darwin":
        return "macOS"
    return s


def default_triplet() -> str:
    env = os.environ.get("VCPKG_DEFAULT_TRIPLET")
    if env:
        return env
    s = detect_system()
    if s == "Windows":
        return "x64-windows"
    if s == "Linux":
        return "x64-linux-dynamic"
    if s == "macOS":
        return "x64-osx"
    return "x64-linux-dynamic"


# ── Resolve vcpkg ────────────────────────────────────────────────


def resolve_vcpkg() -> tuple[Path, Path]:
    """Return (vcpkg_root, vcpkg_executable)."""
    exe_name = "vcpkg.exe" if detect_system() == "Windows" else "vcpkg"

    for var in ("GLOBAL_VCPKG_PATH", "VCPKG_ROOT"):
        val = os.environ.get(var, "")
        if val:
            root = Path(val)
            exe = root / exe_name
            if exe.is_file():
                return root, exe

    found = shutil.which("vcpkg")
    if found:
        exe = Path(found).resolve()
        return exe.parent, exe

    print(c(RED, "Error: vcpkg not found."))
    print("  Set GLOBAL_VCPKG_PATH or VCPKG_ROOT environment variable,")
    print("  or ensure 'vcpkg' is on your PATH.")
    sys.exit(1)


# ── Git helpers ──────────────────────────────────────────────────


def run(
    cmd: list[str],
    *,
    cwd: Path | None = None,
    capture: bool = False,
    check: bool = True,
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd,
        cwd=cwd,
        text=True,
        capture_output=capture,
        check=check,
    )


def clone_or_update_registry() -> Path:
    git_dir = REGISTRY_DIR / ".git"
    if not git_dir.is_dir():
        log("Cloning ArcVideoRegistry...", CYAN)
        run(["git", "clone", "--depth", "1", REGISTRY_URL, str(REGISTRY_DIR)])
    else:
        log("Updating ArcVideoRegistry...", CYAN)
        run(["git", "pull", "--ff-only"], cwd=REGISTRY_DIR)
    ports_dir = REGISTRY_DIR / "ports"
    log(f"Registry: {REGISTRY_DIR}", GREEN)
    return ports_dir


# ── Port discovery ───────────────────────────────────────────────


class Port:
    def __init__(
        self, name: str, version: str, directory: Path, installed: bool = False
    ):
        self.name = name
        self.version = version
        self.directory = directory
        self.installed = installed


def discover_ports(ports_dir: Path) -> list[Port]:
    ports: list[Port] = []
    for d in sorted(ports_dir.iterdir()):
        if not d.is_dir():
            continue
        vcpkg_json = d / "vcpkg.json"
        if not vcpkg_json.is_file():
            continue
        with open(vcpkg_json) as f:
            meta = json.load(f)
        name = meta.get("name", d.name)
        version = meta.get("version") or meta.get("version-string") or "?"
        ports.append(Port(name, version, d))
    return ports


def detect_installed(vcpkg_exe: Path, ports: list[Port]) -> None:
    result = run([str(vcpkg_exe), "list", "--x-full-desc"], capture=True, check=False)
    installed_names: set[str] = set()
    for line in result.stdout.splitlines():
        m = re.match(r"^([a-z0-9\-]+):", line)
        if m:
            installed_names.add(m.group(1))
    for p in ports:
        p.installed = p.name in installed_names


# ── Display & selection ──────────────────────────────────────────


def display_ports(ports: list[Port]) -> None:
    print()
    hdr = f"  {'#':>3}  {'Package':<24} {'Version':<10} {'Installed'}"
    print(c(WHITE, hdr))
    print(
        c(
            DIM,
            f"  {'---':>3}  {'------------------------':<24} {'----------':<10} {'---------'}",
        )
    )
    for i, p in enumerate(ports, 1):
        idx = f"{i:>3}"
        name = f"{p.name:<24}"
        ver = f"{p.version:<10}"
        if p.installed:
            mark = c(GREEN, "Yes")
        else:
            mark = c(YELLOW, "No")
        print(f"  {idx}  {name} {c(DIM, ver)} {mark}")
    print()


def select_ports(ports: list[Port], action: str) -> list[Port]:
    prompt = f"  Enter port numbers to {action} (comma-separated, or 'a' for all): "
    user_input = input(c(CYAN, prompt)).strip()
    if user_input.lower() == "a":
        return list(ports)
    indices: list[int] = []
    for tok in re.split(r"[,\s]+", user_input):
        if tok.isdigit():
            idx = int(tok) - 1
            if 0 <= idx < len(ports):
                indices.append(idx)
    return [ports[i] for i in indices]


# ── Execute install / uninstall ──────────────────────────────────


def execute(
    vcpkg_exe: Path, ports: list[Port], ports_dir: Path, triplet: str, action: str
) -> None:
    overlay_arg = f"--overlay-ports={ports_dir}"
    specs = [f"{p.name}:{triplet}" for p in ports]

    if action == "uninstall":
        for spec in specs:
            log(f"Uninstalling {spec}...", YELLOW)
            result = run([str(vcpkg_exe), "remove", spec], check=False)
            if result.returncode != 0:
                log(f"FAILED: {spec}", RED)
                sys.exit(1)
            log(f"OK: {spec}", GREEN)
            print()
    else:
        # Remove all selected ports first (single command to avoid cascade issues)
        log("Removing selected ports (if present)...", DIM)
        run([str(vcpkg_exe), "remove", "--recurse", *specs], check=False, capture=True)

        # Install all selected ports in one command so vcpkg resolves deps correctly
        log(f"Installing: {', '.join(specs)}...", CYAN)
        result = run([str(vcpkg_exe), "install", overlay_arg, *specs], check=False)
        if result.returncode != 0:
            log("FAILED to install overlay ports!", RED)
            sys.exit(1)
        for spec in specs:
            log(f"OK: {spec}", GREEN)


# ── Main ─────────────────────────────────────────────────────────


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Install/uninstall ArcVideoRegistry overlay-port packages via vcpkg."
    )
    parser.add_argument(
        "--uninstall",
        action="store_true",
        help="Uninstall selected ports instead of installing",
    )
    parser.add_argument(
        "--all", "-a", action="store_true", help="Process all ports without prompting"
    )
    args = parser.parse_args()

    action = "uninstall" if args.uninstall else "install"

    banner()

    system = detect_system()
    triplet = default_triplet()
    vcpkg_root, vcpkg_exe = resolve_vcpkg()

    print(f"  System : {system}")
    print(f"  vcpkg  : {vcpkg_exe}")
    print(f"  Triplet: {triplet}")
    print()

    ports_dir = clone_or_update_registry()
    ports = discover_ports(ports_dir)

    if not ports:
        log("No ports found in registry.", YELLOW)
        return

    detect_installed(vcpkg_exe, ports)
    display_ports(ports)

    if args.all:
        selected = list(ports)
    else:
        selected = select_ports(ports, action)

    if not selected:
        log("Nothing selected.", YELLOW)
        return

    print()
    execute(vcpkg_exe, selected, ports_dir, triplet, action)
    log("Complete.", GREEN)


if __name__ == "__main__":
    main()
