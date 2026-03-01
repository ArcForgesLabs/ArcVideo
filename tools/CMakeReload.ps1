<#
.SYNOPSIS
    Watches for C/C++ source file add/delete/rename and auto-runs cmake configure.
.DESCRIPTION
    Replaces CMakeReload.exe (C# version). Monitors the project tree for source
    file changes (create, delete, rename) and triggers cmake --preset <preset>.
    If a generate-vs-sln.cmake script exists in the build dir, it runs that too.
.PARAMETER ProjectRoot
    Path to the CMake project root. Detected automatically if omitted.
.PARAMETER Preset
    CMake preset to use (default: windows-vs).
.PARAMETER DebounceMs
    Debounce time in milliseconds (default: 1500, min: 500).
.PARAMETER Verbose
    Show cmake stdout output.
#>
param(
    [string]$ProjectRoot,
    [string]$Preset = "windows-vs",
    [int]$DebounceMs = 1500,
    [switch]$Verbose
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$SourceExtensions = @('.h','.hpp','.hxx','.hh','.cpp','.cxx','.cc','.c','.inl','.ipp')
$ExcludeDirs = @('build','.git','.vs','.idea','.cache','node_modules','__pycache__')

function Find-ProjectRoot {
    # 1. Explicit parameter
    if ($ProjectRoot -and (Test-Path "$ProjectRoot\CMakeLists.txt")) {
        return (Resolve-Path $ProjectRoot).Path
    }

    # 2. Git repo root
    try {
        $gitRoot = (git rev-parse --show-toplevel 2>$null)
        if ($gitRoot -and (Test-Path "$gitRoot\CMakeLists.txt")) {
            return (Resolve-Path $gitRoot).Path
        }
    } catch {}

    # 3. Script directory
    $scriptDir = $PSScriptRoot
    if ($scriptDir -and (Test-Path "$scriptDir\CMakeLists.txt")) {
        return $scriptDir
    }

    # 4. Parent of script directory (tools/ → project root)
    $parentDir = Split-Path $scriptDir -Parent
    if ($parentDir -and (Test-Path "$parentDir\CMakeLists.txt")) {
        return $parentDir
    }

    # 5. Current working directory
    $cwd = Get-Location
    if (Test-Path "$cwd\CMakeLists.txt") {
        return $cwd.Path
    }

    return $null
}

function Write-Log {
    param([string]$Message, [ConsoleColor]$Color = 'White')
    $time = Get-Date -Format "HH:mm:ss"
    Write-Host "[$time] " -ForegroundColor DarkGray -NoNewline
    Write-Host $Message -ForegroundColor $Color
}

function Test-SourceFile([string]$Path) {
    $ext = [System.IO.Path]::GetExtension($Path)
    return $ext -and ($SourceExtensions -contains $ext)
}

function Test-TempFile([string]$Path) {
    $name = [System.IO.Path]::GetFileName($Path)
    return ($name -match '~') -or ($name -match '\.TMP$')
}

function Test-ExcludedPath([string]$FullPath, [string]$Root) {
    $relative = [System.IO.Path]::GetRelativePath($Root, $FullPath)
    $parts = $relative -split '[/\\]'
    foreach ($part in $parts) {
        if ($ExcludeDirs -contains $part) { return $true }
    }
    return $false
}

function Test-RelevantSourceFile([string]$FullPath, [string]$Root) {
    return (Test-SourceFile $FullPath) -and
           (-not (Test-TempFile $FullPath)) -and
           (-not (Test-ExcludedPath $FullPath $Root))
}

function Get-RelPath([string]$FullPath, [string]$Root) {
    return [System.IO.Path]::GetRelativePath($Root, $FullPath)
}

function Find-BuildDir([string]$Root, [string]$PresetName) {
    $config = $PresetName -replace '^windows-', ''
    $dir = Join-Path $Root "build" $config
    if (Test-Path $dir) { return $dir }
    return Join-Path $Root "build"
}

function Invoke-CMakeReload([string]$Root, [string]$PresetName) {
    Write-Host
    Write-Log "▶ Running cmake configure..." Cyan
    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    $cmakeArgs = @("--preset", $PresetName)
    $result = & cmake @cmakeArgs 2>&1
    if ($LASTEXITCODE -ne 0) {
        $result | Where-Object { $_ -is [System.Management.Automation.ErrorRecord] } | ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
        Write-Log "✗ cmake configure failed!" Red
        return
    }
    if ($Verbose) { $result | ForEach-Object { Write-Host "    $_" } }

    $buildDir = Find-BuildDir $Root $PresetName
    $genScript = Join-Path $buildDir "generate-vs-sln.cmake"
    if (Test-Path $genScript) {
        Write-Log "▶ Regenerating VS solution..." Cyan
        $slnResult = & cmake -P "$genScript" 2>&1
        if ($LASTEXITCODE -ne 0) {
            $slnResult | Where-Object { $_ -is [System.Management.Automation.ErrorRecord] } | ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
            Write-Log "✗ sln generation failed!" Red
            return
        }
        if ($Verbose) { $slnResult | ForEach-Object { Write-Host "    $_" } }
    }

    $sw.Stop()
    Write-Log ("✓ Done in {0:F1}s" -f $sw.Elapsed.TotalSeconds) Green
    Write-Host
}

# --- Main ---

$root = Find-ProjectRoot
if (-not $root) {
    Write-Error "Error: No CMakeLists.txt found.`n  Run from a CMake project root, a subdirectory (e.g. tools/), or pass -ProjectRoot."
    exit 1
}

if ($DebounceMs -lt 500) { $DebounceMs = 500 }

Write-Host "╔══════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║          CMakeReload v2.0.0 (PS1)        ║" -ForegroundColor Cyan
Write-Host "╚══════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host "  Project : $root"
Write-Host "  Preset  : $Preset"
Write-Host "  Debounce: ${DebounceMs}ms"
Write-Host "  Watch   : $($SourceExtensions -join ', ')"
Write-Host

Write-Log "Watching for source file changes... Press Ctrl+C to stop." Green
Write-Host

$watcher = [System.IO.FileSystemWatcher]::new($root)
$watcher.IncludeSubdirectories = $true
$watcher.NotifyFilter = [System.IO.NotifyFilters]::FileName -bor [System.IO.NotifyFilters]::DirectoryName
$watcher.EnableRaisingEvents = $true

$state = @{
    RebuildPending = $false
    IsRunning      = $false
    LastTrigger    = [datetime]::MinValue
}

$onCreated = {
    $path = $Event.SourceEventArgs.FullPath
    $root = $Event.MessageData.Root
    $state = $Event.MessageData.State
    if (-not (Test-RelevantSourceFile $path $root)) { return }
    Write-Log "  + $(Get-RelPath $path $root)" Green
    $state.RebuildPending = $true
    $state.LastTrigger = [datetime]::Now
}

$onDeleted = {
    $path = $Event.SourceEventArgs.FullPath
    $root = $Event.MessageData.Root
    $state = $Event.MessageData.State
    if (-not (Test-RelevantSourceFile $path $root)) { return }
    Write-Log "  - $(Get-RelPath $path $root)" Red
    $state.RebuildPending = $true
    $state.LastTrigger = [datetime]::Now
}

$onRenamed = {
    $e = $Event.SourceEventArgs
    $root = $Event.MessageData.Root
    $state = $Event.MessageData.State
    $oldOk = Test-RelevantSourceFile $e.OldFullPath $root
    $newOk = Test-RelevantSourceFile $e.FullPath $root
    if (-not $oldOk -or -not $newOk) { return }
    Write-Log "  ~ $(Get-RelPath $e.OldFullPath $root) → $(Get-RelPath $e.FullPath $root)" Yellow
    $state.RebuildPending = $true
    $state.LastTrigger = [datetime]::Now
}

$msgData = @{ Root = $root; State = $state }

Register-ObjectEvent $watcher Created -Action ([scriptblock]$onCreated)  -MessageData $msgData | Out-Null
Register-ObjectEvent $watcher Deleted -Action ([scriptblock]$onDeleted)  -MessageData $msgData | Out-Null
Register-ObjectEvent $watcher Renamed -Action ([scriptblock]$onRenamed)  -MessageData $msgData | Out-Null

try {
    while ($true) {
        Start-Sleep -Milliseconds 200

        if (-not $state.RebuildPending -or $state.IsRunning) { continue }
        if (([datetime]::Now - $state.LastTrigger).TotalMilliseconds -lt $DebounceMs) { continue }

        $state.RebuildPending = $false
        $state.IsRunning = $true

        Push-Location $root
        try { Invoke-CMakeReload $root $Preset }
        finally {
            Pop-Location
            $state.IsRunning = $false
        }
    }
}
finally {
    Get-EventSubscriber | Unregister-Event
    $watcher.Dispose()
    Write-Host
    Write-Log "Stopped." Yellow
}
