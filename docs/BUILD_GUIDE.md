# ArcVideo - Windows Build Guide

## Overview

ArcVideo uses **CMake 4.2+** with the **Visual Studio 2026** generator. Dependencies are managed through **vcpkg** (classic mode) and a manually installed **Qt6**.

The VS generator is **multi-config** — a single `cmake --preset windows-vs` produces one `.sln` containing Debug, Release, and all other configurations.

> **CLion users**: FASTBuild presets (`windows-debug`, `windows-release`) are also available in `CMakePresets.json`. CLion auto-detects them — just open the project and select a preset. No additional setup needed beyond the environment variables and vcpkg packages below.

---

## Prerequisites

| Tool | Version | Install |
|------|---------|---------|
| **Visual Studio 2026** | Latest | [visualstudio.microsoft.com](https://visualstudio.microsoft.com/) |
| **CMake** | ≥ 4.2 | `scoop install cmake` |
| **vcpkg** | Latest | `git clone https://github.com/microsoft/vcpkg.git` → `bootstrap-vcpkg.bat` |
| **Qt6** | ≥ 6.2 | Manual install from [qt.io](https://www.qt.io/) (all modules except QtWebEngine) |
| **Git** | Latest | `scoop install git` |

---

## Step 1: Environment Variables

Set the following **permanent User environment variables** (PowerShell as Administrator is NOT required):

```powershell
# vcpkg root (use a custom name to avoid VS hijacking VCPKG_ROOT)
[System.Environment]::SetEnvironmentVariable("GLOBAL_VCPKG_PATH", "C:\path\to\vcpkg", "User")

# Qt6 install path (the directory containing bin/, lib/, include/)
[System.Environment]::SetEnvironmentVariable("CMAKE_PREFIX_PATH", "C:\Qt\6.x.x\msvc2022_64", "User")

# Windows SDK resource compiler path (required for FASTBuild / CLion)
# Find yours: Get-ChildItem "C:\Program Files (x86)\Windows Kits\10\bin" -Recurse -Filter "rc.exe"
[System.Environment]::SetEnvironmentVariable("WindowsKitRC", "C:\Program Files (x86)\Windows Kits\10\bin\10.0.xxxxx.0\x64", "User")

# FASTBuild cache directory (required for FASTBuild / CLion)
[System.Environment]::SetEnvironmentVariable("FASTBUILD_CACHE_PATH", "$env:LOCALAPPDATA\FASTBuild\Cache", "User")
```

| Variable | Required for | Why |
|----------|-------------|-----|
| `GLOBAL_VCPKG_PATH` | All | VS overrides `VCPKG_ROOT` with its bundled vcpkg. Custom name avoids hijacking. |
| `CMAKE_PREFIX_PATH` | All | Allows CMake to find Qt6 without hardcoding paths. |
| `WindowsKitRC` | FASTBuild / CLion | FASTBuild needs `rc.exe` on PATH. The SDK bin directory isn't on PATH by default. |
| `FASTBUILD_CACHE_PATH` | FASTBuild / CLion | FASTBuild compilation cache location. Must be a permanent env var. |

> **Important**: After setting environment variables, restart your terminal / IDE for them to take effect.

---

## Step 2: Install vcpkg Dependencies

Install the following packages in **classic mode** (the project does not use a vcpkg manifest):

```powershell
cd $env:GLOBAL_VCPKG_PATH

vcpkg install --triplet x64-windows `
  opencolorio `
  openimageio `
  openexr `
  imath `
  ffmpeg[avcodec,avdevice,avfilter,avformat,swresample,swscale] `
  portaudio `
  crashpad `
  zlib `
  libpng
```

### Full dependency list

| Package | Version (tested) | Purpose |
|---------|-----------------|---------|
| `opencolorio` | 2.5.1 | Color management |
| `openimageio` | 3.0.9 | Image I/O |
| `openexr` | 3.4.5 | HDR image format |
| `imath` | 3.2.2 | Math library for VFX |
| `ffmpeg` | 8.0.1 | Video/audio codec |
| `portaudio` | 19.7 | Audio playback |
| `crashpad` | 2024-04-11 | Crash reporting |
| `zlib` | 1.3.1 | Compression |
| `libpng` | 1.6.55 | PNG image support |

Additional packages are pulled in automatically as transitive dependencies (e.g., `libjpeg-turbo`, `tiff`, `yaml-cpp`, `fmt`, `pystring`, etc.).

---

## Step 3: Clone and Initialize

```powershell
git clone --recursive https://github.com/arcvideo-editor/olive.git
cd olive
```

If you already cloned without `--recursive`:
```powershell
git submodule update --init --recursive
```

### Submodules

| Submodule | Source |
|-----------|--------|
| KDDockWidgets | [KDAB/KDDockWidgets](https://github.com/KDAB/KDDockWidgets) |
| OpenTimelineIO | [AcademySoftwareFoundation/OpenTimelineIO](https://github.com/AcademySoftwareFoundation/OpenTimelineIO) |

---

## Step 4: Configure

```powershell
cmake --preset windows-vs
```

This generates `build/vs/arcvideo-editor.sln` with all configurations (Debug, Release, RelWithDebInfo, MinSizeRel).

---

## Step 5: Build

### Visual Studio IDE

1. Open `build/vs/arcvideo-editor.sln`
2. Set `arcvideo-editor` as the startup project
3. Switch Debug / Release from the toolbar dropdown
4. Build with `Ctrl+B`

> **Tip**: The `ZERO_CHECK` target automatically re-runs CMake when `CMakeLists.txt` changes. No manual reconfigure needed.

### Command Line

```powershell
# Debug
cmake --build build/vs --config Debug

# Release
cmake --build build/vs --config Release
```

### Output

| Configuration | Executable | Crash handler |
|--------------|-----------|---------------|
| Debug | `build/vs/bin/Debug/arcvideo-editor.exe` | `build/vs/bin/Debug/arcvideo-crashhandler.exe` |
| Release | `build/vs/bin/Release/arcvideo-editor.exe` | `build/vs/bin/Release/arcvideo-crashhandler.exe` |

---

## Troubleshooting

### `Could not find toolchain file: "/scripts/buildsystems/vcpkg.cmake"`

`GLOBAL_VCPKG_PATH` environment variable is not set or not visible. Verify:
```powershell
[System.Environment]::GetEnvironmentVariable("GLOBAL_VCPKG_PATH", "User")
```

### `Could not find a package configuration file provided by "OpenColorIO"`

vcpkg packages not installed, or VS is using its bundled vcpkg instead of yours. This happens when `VCPKG_ROOT` is hijacked by Visual Studio. The project uses `GLOBAL_VCPKG_PATH` to avoid this — make sure it's set.

### CMake minimum version error

This project requires **CMake 4.2+**. Update with `scoop update cmake`.

---

## Build Configuration Summary

| Setting | Value |
|---------|-------|
| CMake minimum | 4.2 |
| C++ Standard | C++20 |
| Generator | Visual Studio 18 2026 |
| Triplet | x64-windows |
| Debug format | `/Zi` (PDB, default) |
| Configurations | All configs in one `.sln` |
| Submodules | KDDockWidgets, OpenTimelineIO |
