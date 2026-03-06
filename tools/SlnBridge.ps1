<#
.SYNOPSIS
    Configure the CMake project and build third-party dependencies.
.DESCRIPTION
    SlnBridge automates the two-step workflow:
      1. cmake --preset <preset>   → generates the VS solution (.slnx)
      2. cmake --build             → builds only the third-party libraries
         (OpenTimelineIO) for both Debug and Release configs
         so the solution opens without errors.

    After running this script, open the .slnx in Visual Studio and develop
    normally — only the arcvideo-editor targets need manual building.
.PARAMETER ProjectRoot
    Path to the CMake project root. Detected automatically if omitted.
.PARAMETER Preset
    CMake configure preset (default: windows-vs).
.PARAMETER Configs
    Build configurations for third-party libs (default: Debug, Release).
    Can pass multiple, e.g. -Configs Debug,Release
.PARAMETER SkipConfigure
    Skip cmake configure and only build third-party libs.
.PARAMETER Open
    Open the .slnx in Visual Studio after building.
#>
param(
    [string]$ProjectRoot,
    [string]$Preset = "windows-vs",
    [ValidateSet("Debug","Release","RelWithDebInfo","MinSizeRel")]
    [string[]]$Configs = @("Debug", "Release"),
    [switch]$SkipConfigure,
    [switch]$Open
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Find-ProjectRoot {
    if ($ProjectRoot -and (Test-Path "$ProjectRoot\CMakeLists.txt")) {
        return (Resolve-Path $ProjectRoot).Path
    }

    try {
        $gitRoot = (git rev-parse --show-toplevel 2>$null)
        if ($gitRoot -and (Test-Path "$gitRoot\CMakeLists.txt")) {
            return (Resolve-Path $gitRoot).Path
        }
    } catch {}

    $scriptDir = $PSScriptRoot
    if ($scriptDir -and (Test-Path "$scriptDir\CMakeLists.txt")) {
        return $scriptDir
    }

    $parentDir = Split-Path $scriptDir -Parent
    if ($parentDir -and (Test-Path "$parentDir\CMakeLists.txt")) {
        return $parentDir
    }

    $cwd = (Get-Location).Path
    if (Test-Path "$cwd\CMakeLists.txt") {
        return $cwd
    }

    return $null
}

function Write-Log {
    param([string]$Message, [ConsoleColor]$Color = 'White')
    $time = Get-Date -Format "HH:mm:ss"
    Write-Host "[$time] " -ForegroundColor DarkGray -NoNewline
    Write-Host $Message -ForegroundColor $Color
}

function Get-BuildDir([string]$Root, [string]$PresetName) {
    $presetsFile = Join-Path $Root "CMakePresets.json"
    if (Test-Path $presetsFile) {
        $presets = Get-Content $presetsFile -Raw | ConvertFrom-Json
        $match = $presets.configurePresets | Where-Object { $_.name -eq $PresetName }
        if ($match -and $match.binaryDir) {
            $dir = $match.binaryDir -replace '\$\{sourceDir\}', $Root
            return $dir
        }
    }
    # Fallback convention
    $config = $PresetName -replace '^windows-', ''
    return Join-Path $Root "build" $config
}

# --- Main ---

$root = Find-ProjectRoot
if (-not $root) {
    Write-Error "Error: No CMakeLists.txt found.`nRun from a CMake project root, a subdirectory (e.g. tools/), or pass -ProjectRoot."
    exit 1
}

$buildDir = Get-BuildDir $root $Preset
$configList = $Configs -join ', '

Write-Host "╔══════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║          SlnBridge v1.0.0                ║" -ForegroundColor Cyan
Write-Host "╚══════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host "  Project : $root"
Write-Host "  Preset  : $Preset"
Write-Host "  Configs : $configList"
Write-Host "  BuildDir: $buildDir"
Write-Host

$sw = [System.Diagnostics.Stopwatch]::StartNew()

# Step 1: Configure
if (-not $SkipConfigure) {
    Write-Log ">> Step 1: cmake --preset $Preset" Cyan
    $prevDir = Get-Location
    Set-Location $root
    $output = & cmake --preset $Preset 2>&1
    $configExitCode = $LASTEXITCODE
    Set-Location $prevDir
    $output | ForEach-Object { Write-Host "    $_" }
    if ($configExitCode -ne 0) {
        Write-Log "x cmake configure failed!" Red
        exit 1
    }
    Write-Log "ok Configure succeeded" Green
} else {
    Write-Log "-- Skipping configure" Yellow
    if (-not (Test-Path $buildDir)) {
        Write-Error "Build directory not found: $buildDir`nRun without -SkipConfigure first."
        exit 1
    }
}

Write-Host

# Step 2: Build third-party libraries
$thirdPartyTargets = @("opentimelineio", "opentime")

Write-Log ">> Step 2: Building third-party libraries..." Cyan
$targetList = $thirdPartyTargets -join ', '
Write-Log "  Targets: $targetList" White

# 遍历所有的 Config (默认 Debug 和 Release 都跑一遍)
foreach ($cfg in $Configs) {
    Write-Log ">> Building Configuration: [$cfg]" Yellow
    foreach ($target in $thirdPartyTargets) {
        Write-Log "  Building $target ($cfg)..." White
        $output = & cmake --build $buildDir --config $cfg --target $target -- /v:m /nologo 2>&1
        $buildExitCode = $LASTEXITCODE
        foreach ($line in $output) {
            $text = "$line"
            if ($text -match "error") {
                Write-Host "    $text" -ForegroundColor Red
            } elseif ($text -match "warning") {
                Write-Host "    $text" -ForegroundColor Yellow
            } else {
                Write-Host "    $text"
            }
        }
        if ($buildExitCode -ne 0) {
            Write-Log "x Failed to build $target for $cfg!" Red
            exit 1
        }
        Write-Log "  ok $target ($cfg) built" Green
    }
}

$sw.Stop()
Write-Host
$elapsed = "{0:F1}" -f $sw.Elapsed.TotalSeconds
Write-Log "ok All done in ${elapsed}s" Green

# Find .slnx / .sln
$slnFile = Get-ChildItem $buildDir -Filter "*.slnx" -File | Select-Object -First 1
if (-not $slnFile) {
    $slnFile = Get-ChildItem $buildDir -Filter "*.sln" -File | Select-Object -First 1
}

if ($slnFile) {
    Write-Host
    $slnFullPath = $slnFile.FullName
    Write-Log "Solution: $slnFullPath" Cyan
    if ($Open) {
        Write-Log "Opening in Visual Studio..." Cyan
        Start-Process $slnFile.FullName
    } else {
        Write-Log "Run with -Open to launch Visual Studio, or open manually:" White
        $slnPath = $slnFile.FullName
        Write-Host "    $slnPath" -ForegroundColor White
    }
}

Write-Host "`nPress any key to exit..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
