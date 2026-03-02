<div align="center">

# 🧱 ArcVideoFoundation

**Pure C++20 Core Library for AI-Powered Video Editing**

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?logo=cplusplus)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-4.2+-064F8C?logo=cmake)](https://cmake.org/)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20|%20Linux%20|%20macOS-lightgrey)]()

---

*The foundational "fat core" powering [ArcVideo](https://github.com/ArcForgesLabs/ArcVideo) — zero UI dependency, maximum portability.*

</div>

---

## What is ArcVideoFoundation?

ArcVideoFoundation is the **platform-agnostic core library** extracted from the ArcVideo non-linear video editor. It provides all the fundamental data types, algorithms, and abstractions needed for professional video editing — without any dependency on Qt or any other UI framework.

### 🎯 Design Philosophy

| Principle | Description |
|-----------|-------------|
| **Zero UI Dependency** | Pure Standard C++20. No Qt, no GUI frameworks, no window system coupling. |
| **Fat Core** | Handles the heavy lifting: time math, color processing, audio buffers, SIMD optimization. |
| **FFI Ready** | Designed for future `extern "C"` boundary — enabling consumption from C#, Rust, Python, etc. |
| **Subdirectory Friendly** | Works standalone or as `add_subdirectory()` in a parent CMake project. |

---

## Architecture

```
foundation/
├── include/arcvideo/foundation/     # Public API headers
│   ├── foundation.h                  # Umbrella header
│   ├── render/                       # Audio/video data types
│   │   ├── audioparams.h             #   Audio channel layout, sample rate
│   │   ├── pixelformat.h             #   Pixel format definitions
│   │   ├── samplebuffer.h            #   Audio sample buffer management
│   │   └── sampleformat.h            #   Audio sample format (int16, float, etc.)
│   └── util/                         # Core utilities
│       ├── bezier.h                  #   Bézier curve evaluation
│       ├── color.h                   #   RGBA color + HSL conversion
│       ├── cpuoptimize.h             #   SSE/NEON SIMD dispatch
│       ├── log.h                     #   Lightweight logging
│       ├── math.h                    #   Math helpers
│       ├── rational.h                #   Exact rational numbers (via FFmpeg AVRational)
│       ├── stringutils.h             #   String formatting, split, trim
│       ├── timecodefunctions.h       #   Timecode ↔ time conversion (NDF, DF, seconds)
│       ├── timerange.h               #   Time ranges and range lists
│       └── value.h                   #   Type-erased value container
├── src/                              # Implementation files
├── docs/                             # Build guide
├── tools/                            # SlnBridge.ps1 build automation
├── CMakeLists.txt                    # Standalone CMake project
└── CMakePresets.json                 # VS 2026 + FASTBuild presets
```

---

## Quick Start

### Prerequisites

- **CMake 4.2+**, **Visual Studio 2026**, **vcpkg**
- vcpkg packages: `imath`, `ffmpeg`

### Build

```powershell
# Configure
cmake --preset windows-vs

# Build
cmake --build build/vs --config Debug

# Or use the one-command tool
.\tools\SlnBridge.ps1 -Open
```

See [docs/BUILD_GUIDE.md](docs/BUILD_GUIDE.md) for the full setup guide.

### Use in Your Project

```cmake
add_subdirectory(foundation)
target_link_libraries(your-target PRIVATE ArcVideo::Foundation)
```

```cpp
#include <arcvideo/foundation/foundation.h>

using namespace arcvideo::foundation;

rational fps(24000, 1001);  // 23.976 fps
TimeRange clip(rational(0), rational(10));
Color red(1.0, 0.0, 0.0, 1.0);
```

---

## Why Extract a Core Library?

ArcVideoFoundation exists to enable **complete UI/core separation**:

```
┌─────────────────────────────────────────┐
│           UI Layer (Swappable)          │
│  ┌─────────────┐  ┌──────────────────┐ │
│  │  Qt Widgets  │  │  Avalonia (C#)   │ │
│  │  (Current)   │  │  (Future)        │ │
│  └──────┬───────┘  └────────┬─────────┘ │
│         │     extern "C"    │           │
│  ───────┴───────────────────┴────────── │
│         ArcVideoFoundation              │
│   Pure C++20 · Zero UI · SIMD · FFI    │
└─────────────────────────────────────────┘
```

- **Now**: Qt widgets call foundation directly via C++ linkage
- **Future**: C# Avalonia UI consumes foundation through a flat C API (Silk.NET / P/Invoke)
- **Result**: Zero-copy GPU texture sharing, UI migration without core rewrite

---

## Dependencies

| Library | Purpose | Required |
|---------|---------|----------|
| [Imath](https://github.com/AcademySoftwareFoundation/Imath) | VFX-standard math (half, vectors) | ✅ |
| [FFmpeg](https://ffmpeg.org/) (libavutil) | Rational numbers, channel layouts | ✅ |
| [OpenTime](https://github.com/AcademySoftwareFoundation/OpenTimelineIO) | RationalTime interop | Optional |

---

## License

This project is a fork of [Olive Video Editor](https://github.com/olive-editor/olive) and is licensed under the **GNU General Public License v3.0**. See [LICENSE](LICENSE) for details.

Copyright (C) 2023 Olive Studios LLC and contributors.
