

***

```markdown
# Layered CodeQL Scanning with FASTBuild and CMakePresets

This guide demonstrates how to achieve isolated, layered CodeQL scanning for a C++ Monorepo. By leveraging **CMakePresets**, **sequential matrix execution**, and **natural build caching (FASTBuild)**, we can trick CodeQL into analyzing only the incremental layers without writing any hacky CMake configurations.

## 🚨 FAQ: Will it run automatically upon `git push`?

If your `.yml` file contains `on: push`, it will trigger immediately in the background whenever you push.
If you **do not want it to run automatically** (e.g., you only want to trigger it manually via the GitHub UI), you should use **`on: workflow_dispatch`** instead.

---

## 📦 Demo Directory Structure

Assume the following minimal project structure for this demonstration:

```text
C:\MyFile\ArcVideo\
 ├── CMakeLists.txt
 ├── CMakePresets.json
 ├── .github\workflows\demo.yml
 └── app\
     ├── services\services.cpp
     ├── viewmodels\viewmodels.cpp
     └── desktop\desktop.cpp
```

---

## 1. `CMakeLists.txt` (Minimal Architecture)

Define the three-tier dependency chain. CMake inherently knows the correct build order.

```cmake
cmake_minimum_required(VERSION 3.20)
project(ArcVideoDemo CXX)

# Layer 1 (Bottom): No dependencies
add_library(ArcVideo_Services STATIC app/services/services.cpp)

# Layer 2 (Middle): Depends on Services
add_library(ArcVideo_ViewModels STATIC app/viewmodels/viewmodels.cpp)
target_link_libraries(ArcVideo_ViewModels PUBLIC ArcVideo_Services)

# Layer 3 (Top): Depends on ViewModels
add_library(ArcVideo_Desktop STATIC app/desktop/desktop.cpp)
target_link_libraries(ArcVideo_Desktop PUBLIC ArcVideo_ViewModels)
```

---

## 2. `CMakePresets.json` (Targeted Build Directives)

Define targeted build presets so that each layer can be invoked individually.

```json
{
  "version": 3,
  "configurePresets":[
    {
      "name": "linux-ci",
      "generator": "Ninja", 
      "binaryDir": "${sourceDir}/build/linux-ci"
    }
  ],
  "buildPresets":[
    {
      "name": "scan-services",
      "configurePreset": "linux-ci",
      "targets": ["ArcVideo_Services"]
    },
    {
      "name": "scan-viewmodels",
      "configurePreset": "linux-ci",
      "targets": ["ArcVideo_ViewModels"]
    },
    {
      "name": "scan-desktop",
      "configurePreset": "linux-ci",
      "targets": ["ArcVideo_Desktop"]
    }
  ]
}
```

---

## 3. `demo.yml` (The Ultimate CI Workflow)

**⚠️ Crucial Note:** 
Do not chain multiple `init` and `analyze` steps consecutively within a single job! CodeQL uses fixed temporary directories by default, and running it multiple times sequentially in the same job will cause database corruption/conflicts.

**The optimal approach:** Use a `matrix` combined with `max-parallel: 1` to force sequential queuing. They will execute sequentially on the exact same self-hosted runner, sharing the exact same physical cache block.

```yaml
name: "CodeQL Layered Scan Demo"

on:
  # Disables automatic triggers on push. 
  # Allows manual execution via the GitHub Actions UI.
  workflow_dispatch: 

jobs:
  # Step 0: Clear the dedicated CodeQL cache (Ensures the first layer does not hit the cache)
  prepare-cache:
    runs-on: [self-hosted, linux]
    steps:
      - name: Clear FastBuild CodeQL Cache
        # Assuming this is your dedicated cache directory for CodeQL
        run: rm -rf /cache/fastbuild_codeql && mkdir -p /cache/fastbuild_codeql

  # Step 1: Execute targeted scans sequentially
  codeql-scan:
    needs: prepare-cache # Wait for the cache clearance to finish
    runs-on: [self-hosted, linux]
    
    env:
      # Force the use of the clean, isolated cache directory
      FASTBUILD_CACHE_PATH: /cache/fastbuild_codeql 
      
    strategy:
      fail-fast: false
      # 👈 THE SECRET WEAPON: Forces the runner to execute the matrix sequentially
      # This guarantees that the cache state carries over consecutively.
      max-parallel: 1 
      matrix:
        # STRICTLY order this from the bottom layer to the top layer!
        target_preset:[scan-services, scan-viewmodels, scan-desktop]

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Configure CMake
        run: cmake --preset linux-ci

      - name: Initialize CodeQL
        uses: github/codeql-action/init@v4
        with:
          languages: cpp

      # 🎯 Where the magic happens:
      # Round 1 (scan-services): Real full build -> Cached -> Analyzed.
      # Round 2 (scan-viewmodels): Bottom layer hits cache (skipped) -> Real build for ViewModels -> Analyzed.
      # Round 3 (scan-desktop): Bottom & Middle layers hit cache (skipped) -> Real build for Desktop -> Analyzed.
      - name: Build Target
        run: cmake --build --preset ${{ matrix.target_preset }}

      - name: Perform CodeQL Analysis
        uses: github/codeql-action/analyze@v4
        with:
          category: "/module:${{ matrix.target_preset }}"
```



```
