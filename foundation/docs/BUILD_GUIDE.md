# ArcVideoFoundation - Windows Build Guide

## Overview

ArcVideoFoundation is a **pure C++20 library** with zero Qt dependency. It uses **CMake 4.2+** with the **Visual Studio 2026** generator. Dependencies are managed through **vcpkg** (classic mode).

The VS generator is **multi-config** — a single `cmake --preset windows-vs` produces one `.slnx` containing Debug, Release, and all other configurations.

> **CLion users**: FASTBuild presets (`windows-debug`, `windows-release`) are also available in `CMakePresets.json`. CLion auto-detects them — just open the project and select a preset.

---

## Prerequisites

| Tool | Version | Install |
|------|---------|---------|
| **Visual Studio 2026** | Latest | [visualstudio.microsoft.com](https://visualstudio.microsoft.com/) |
| **CMake** | ≥ 4.2 | `scoop install cmake` |
| **vcpkg** | Latest | `git clone https://github.com/microsoft/vcpkg.git` → `bootstrap-vcpkg.bat` |
| **Git** | Latest | `scoop install git` |

---

## Step 1: Environment Variables

Set the following **permanent User environment variables**:

```powershell
# vcpkg root (use a custom name to avoid VS hijacking VCPKG_ROOT)
[System.Environment]::SetEnvironmentVariable("GLOBAL_VCPKG_PATH", "C:\path\to\vcpkg", "User")

# Windows SDK resource compiler path (required for FASTBuild / CLion)
[System.Environment]::SetEnvironmentVariable("WindowsKitRC", "C:\Program Files (x86)\Windows Kits\10\bin\10.0.xxxxx.0\x64", "User")
```

| Variable | Required for | Why |
|----------|-------------|-----|
| `GLOBAL_VCPKG_PATH` | All | VS overrides `VCPKG_ROOT` with its bundled vcpkg. Custom name avoids hijacking. |
| `WindowsKitRC` | FASTBuild / CLion | FASTBuild needs `rc.exe` on PATH. |

> **Important**: After setting environment variables, restart your terminal / IDE for them to take effect.

---

## Step 2: Install vcpkg Dependencies

```powershell
cd $env:GLOBAL_VCPKG_PATH

vcpkg install --triplet x64-windows `
  imath `
  ffmpeg[avcodec,avdevice,avfilter,avformat,swresample,swscale]
```

### Dependency list

| Package | Purpose |
|---------|---------|
| `imath` | VFX math library (half, vectors) |
| `ffmpeg` | Audio/video codec support (libavutil) |

---

## Step 3: Configure

```powershell
cmake --preset windows-vs
```

This generates `build/vs/arcvideo-foundation.slnx` with all configurations.

---

## Step 4: Build

### Visual Studio IDE

1. Open `build/vs/arcvideo-foundation.slnx`
2. Build with `Ctrl+B`

### Command Line

```powershell
# Debug
cmake --build build/vs --config Debug

# Release
cmake --build build/vs --config Release
```

### Quick Start (SlnBridge)

```powershell
# One-command configure + build
.\tools\SlnBridge.ps1

# Configure, build, and open in VS
.\tools\SlnBridge.ps1 -Open
```

---

## Output

| Configuration | Library |
|--------------|---------|
| Debug | `build/vs/lib/Debug/arcvideo-foundation.lib` |
| Release | `build/vs/lib/Release/arcvideo-foundation.lib` |

---

## Using as a Subdirectory

When consumed by a parent project (e.g., ArcVideo), simply add:

```cmake
add_subdirectory(foundation)
target_link_libraries(your-target PRIVATE arcvideo-foundation)
# Or use the alias:
target_link_libraries(your-target PRIVATE ArcVideo::Foundation)
```

Dependencies (Imath, FFmpeg) are automatically found by the parent project — foundation's `PROJECT_IS_TOP_LEVEL` guard skips redundant `find_package` calls.

---

## Build Configuration Summary

| Setting | Value |
|---------|-------|
| CMake minimum | 4.2 |
| C++ Standard | C++20 |
| Generator | Visual Studio 18 2026 |
| Triplet | x64-windows |
| Qt dependency | **None** |
| Library type | Static |
| Source discovery | `GLOB_RECURSE` with `CONFIGURE_DEPENDS` |
