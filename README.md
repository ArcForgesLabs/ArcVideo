<div align="center">

# 🎬 ArcVideo

**AI-Powered Non-Linear Video Editor**

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?logo=cplusplus)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-4.2+-064F8C?logo=cmake)](https://cmake.org/)
[![Qt6](https://img.shields.io/badge/Qt-6-41CD52?logo=qt)](https://www.qt.io/)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-red.svg)](https://www.gnu.org/licenses/gpl-3.0)

*A fork of [Olive Video Editor](https://github.com/olive-editor/olive), reimagined with AI-driven video orchestration.*

---

</div>

## ✨ About

ArcVideo is a professional-grade, open-source non-linear video editor built on the foundation of [Olive](https://github.com/olive-editor/olive). Our focus is bringing **AI-powered editing orchestration** into a native, high-performance desktop application — enabling intelligent clip arrangement, automated editing workflows, and AI-assisted creative decisions, all without sacrificing the precision and responsiveness of a traditional NLE.

### 🧠 AI Orchestration

> Smart editing powered by AI — let the machine handle the tedious work, you focus on the creative vision.

- **Intelligent Timeline Assembly** — AI-assisted clip placement, trimming, and sequencing
- **Automated Editing Workflows** — Scriptable AI pipelines for repetitive editing tasks
- **Content-Aware Editing** — Scene detection, beat-sync, and smart transitions

### 🎨 Core Features (inherited from Olive)

- GPU-accelerated OpenGL rendering pipeline
- OpenColorIO color management
- Multi-format support via FFmpeg
- Node-based compositing system
- OpenTimelineIO interchange

## 🛠️ Tech Stack

| Component | Technology |
|-----------|-----------|
| Language | C++20 |
| UI Framework | Qt 6 |
| Build System | CMake 4.2+ / Visual Studio 2026 |
| Color | OpenColorIO |
| Image I/O | OpenImageIO, OpenEXR |
| Video/Audio | FFmpeg, PortAudio |
| Package Manager | vcpkg |

## 🚀 Getting Started

### Prerequisites

- **Visual Studio 2026** (or CLion with FASTBuild)
- **CMake ≥ 4.2**
- **vcpkg** with required packages
- **Qt 6.2+**

### Build

```powershell
git clone --recursive https://github.com/aspect-based/arcvideo.git
cd arcvideo
cmake --preset windows-vs
cmake --build build/vs --config Debug
```

> 📖 See [docs/BUILD_GUIDE.md](docs/BUILD_GUIDE.md) for detailed setup instructions.

### Quick Start with SlnBridge

```powershell
# One-command setup: configure + build third-party deps + open VS
.\tools\SlnBridge.ps1 -Open
```

## 📁 Project Structure

```
arcvideo/
├── app/              # Main application source
│   ├── core/         # Core library (math, color, time)
│   ├── node/         # Node-based processing graph
│   ├── render/       # GPU rendering pipeline
│   ├── timeline/     # Timeline logic
│   ├── ui/           # Styles and graphics
│   └── widget/       # Qt UI widgets
├── cmake/            # CMake modules
├── tests/            # Unit tests
├── tools/            # Dev utilities (CMakeReload, SlnBridge)
└── docs/             # Build guides and documentation
```

## 📄 License

ArcVideo is licensed under the [GNU General Public License v3.0](LICENSE).

This project is a fork of [Olive Video Editor](https://github.com/olive-editor/olive) — thanks to the Olive team for their incredible work.

---

<div align="center">
<sub>Built with ❤️ and AI</sub>
</div>
