<#
.SYNOPSIS
    Install or uninstall ArcVideoRegistry overlay-port packages via vcpkg.
.DESCRIPTION
    Dynamically discovers all ports from ArcVideoRegistry, detects which are
    already installed locally, and presents an interactive menu to install or
    uninstall them.
.PARAMETER Action
    install (default) or uninstall.
.PARAMETER All
    Skip the interactive menu and process all ports.
#>
param(
    [ValidateSet("install", "uninstall")]
    [string]$Action = "install",
    [switch]$All
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$RegistryUrl  = "https://github.com/ArcForgesLabs/ArcVideoRegistry.git"
$ScriptRoot   = $PSScriptRoot
$ProjectRoot  = Split-Path $ScriptRoot -Parent
$RegistryDir  = Join-Path (Join-Path $ProjectRoot "build") "vcpkg_registry"

function Write-Log([string]$Message, [ConsoleColor]$Color = [ConsoleColor]::White) {
    $time = Get-Date -Format "HH:mm:ss"
    Write-Host "[$time] " -ForegroundColor DarkGray -NoNewline
    Write-Host $Message -ForegroundColor $Color
}

function Resolve-VcpkgPath {
    foreach ($var in @("VCPKG_ROOT", "GLOBAL_VCPKG_PATH")) {
        $val = [Environment]::GetEnvironmentVariable($var)
        if ($val -and (Test-Path (Join-Path $val "vcpkg.exe"))) { return $val }
    }
    $cmd = Get-Command vcpkg -ErrorAction SilentlyContinue
    if ($cmd) { return Split-Path $cmd.Source -Parent }
    return $null
}

function Get-VcpkgTriplet {
    $val = [Environment]::GetEnvironmentVariable("VCPKG_DEFAULT_TRIPLET")
    if ($val) { return $val }
    if ($env:OS -ne "Windows_NT") { return "x64-linux" }
    return "x64-windows"
}

# ── Resolve vcpkg ────────────────────────────────────────────────
$vcpkgRoot = Resolve-VcpkgPath
if (-not $vcpkgRoot) {
    Write-Error "vcpkg not found. Set VCPKG_ROOT or GLOBAL_VCPKG_PATH."
    exit 1
}
$vcpkgExe = Join-Path $vcpkgRoot "vcpkg.exe"
if (-not (Test-Path $vcpkgExe)) { $vcpkgExe = Join-Path $vcpkgRoot "vcpkg" }
$triplet = Get-VcpkgTriplet

Write-Log ("vcpkg  : " + $vcpkgExe) Cyan
Write-Log ("triplet: " + $triplet) Cyan

# ── Clone / update registry ─────────────────────────────────────
Write-Host ""
$gitDir = Join-Path $RegistryDir ".git"
if (-not (Test-Path $gitDir)) {
    Write-Log "Cloning ArcVideoRegistry..." Cyan
    & git clone --depth 1 $RegistryUrl $RegistryDir 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Error "Failed to clone registry"; exit 1 }
} else {
    Write-Log "Updating ArcVideoRegistry..." Cyan
    Push-Location $RegistryDir
    & git pull --ff-only 2>&1 | Out-Null
    Pop-Location
}
$portsDir = Join-Path $RegistryDir "ports"
Write-Log ("Registry: " + $RegistryDir) Green

# ── Discover ports ───────────────────────────────────────────────
$ports = @()
foreach ($dir in (Get-ChildItem $portsDir -Directory)) {
    $jsonPath = Join-Path $dir.FullName "vcpkg.json"
    $json = Get-Content $jsonPath -Raw | ConvertFrom-Json
    $ver = $json.version
    if (-not $ver) { $ver = $json."version-string" }
    if (-not $ver) { $ver = "?" }
    $ports += [PSCustomObject]@{
        Name    = $json.name
        Version = $ver
        Dir     = $dir.FullName
    }
}

if ($ports.Count -eq 0) {
    Write-Log "No ports found in registry." Yellow
    exit 0
}

# ── Detect installed state ───────────────────────────────────────
$installedRaw = & $vcpkgExe list --x-full-desc 2>&1
$installedNames = @()
foreach ($line in $installedRaw) {
    $lineStr = [string]$line
    if ($lineStr -match "^([a-z0-9\-]+):") {
        $installedNames += $Matches[1]
    }
}

$portList = @()
foreach ($p in $ports) {
    $isInstalled = $installedNames -contains $p.Name
    $p | Add-Member -NotePropertyName Installed -NotePropertyValue $isInstalled -PassThru
    $portList += $p
}

# ── Display ──────────────────────────────────────────────────────
Write-Host ""
Write-Host "  #  Package                  Version    Installed" -ForegroundColor White
Write-Host "  -- ------------------------ ---------- ---------" -ForegroundColor DarkGray
for ($i = 0; $i -lt $portList.Count; $i++) {
    $p = $portList[$i]
    $idx = ("{0,3}" -f ($i + 1))
    $name = ("{0,-24}" -f $p.Name)
    $ver  = ("{0,-10}" -f $p.Version)
    if ($p.Installed) { $mark = "Yes"; $color = [ConsoleColor]::Green }
    else              { $mark = "No";  $color = [ConsoleColor]::Yellow }
    Write-Host ("  " + $idx + " ") -NoNewline -ForegroundColor White
    Write-Host ($name + " ") -NoNewline -ForegroundColor White
    Write-Host ($ver + " ") -NoNewline -ForegroundColor DarkGray
    Write-Host $mark -ForegroundColor $color
}
Write-Host ""

# ── Selection ────────────────────────────────────────────────────
if ($All) {
    $selected = $portList
} else {
    $promptMsg = "Enter port numbers to " + $Action + " (comma-separated, or a for all)"
    Write-Host ("  " + $promptMsg) -ForegroundColor Cyan
    $userInput = Read-Host "  >"

    if ($userInput.Trim().ToLower() -eq "a") {
        $selected = $portList
    } else {
        $indices = $userInput -split "[,\s]+" |
            Where-Object { $_ -match "^\d+$" } |
            ForEach-Object { [int]$_ - 1 }
        $selected = @()
        foreach ($idx in $indices) {
            if ($idx -ge 0 -and $idx -lt $portList.Count) {
                $selected += $portList[$idx]
            }
        }
    }
}

if ($selected.Count -eq 0) {
    Write-Log "Nothing selected." Yellow
    exit 0
}

# ── Execute ──────────────────────────────────────────────────────
Write-Host ""
$overlayArg = "--overlay-ports=" + $portsDir

foreach ($pkg in $selected) {
    $spec = $pkg.Name + ":" + $triplet

    if ($Action -eq "uninstall") {
        Write-Log ("Uninstalling " + $spec + "...") Yellow
        & $vcpkgExe remove $spec 2>&1 | ForEach-Object { Write-Host ("    " + $_) }
    } else {
        Write-Log ("Removing " + $spec + " (if present)...") DarkGray
        & $vcpkgExe remove $spec 2>&1 | Out-Null

        Write-Log ("Installing " + $spec + "...") Cyan
        & $vcpkgExe install $spec $overlayArg 2>&1 | ForEach-Object {
            $text = [string]$_
            if ($text -match "error") {
                Write-Host ("    " + $text) -ForegroundColor Red
            } else {
                Write-Host ("    " + $text)
            }
        }
    }

    if ($LASTEXITCODE -ne 0) {
        Write-Log ("FAILED: " + $spec) Red
        exit 1
    }
    Write-Log ("OK: " + $spec) Green
    Write-Host ""
}

Write-Log "Complete." Green
