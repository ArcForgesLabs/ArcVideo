!include "MUI2.nsh"

!define MUI_ICON "install icon.ico"
!define MUI_UNICON "uninstall icon.ico"

!define APP_NAME "ArcVideo"
!define APP_TARGET "arcvideo-editor"

!define MUI_FINISHPAGE_RUN "$INSTDIR\arcvideo-editor.exe"

SetCompressor lzma

Name ${APP_NAME}

ManifestDPIAware true
Unicode true

!ifdef X64
InstallDir "$PROGRAMFILES64\${APP_NAME}"
!else
InstallDir "$PROGRAMFILES32\${APP_NAME}"
!endif

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE LICENSE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_RUN_TEXT "Run ${APP_NAME}"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchArcVideo"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section "ArcVideo"
    SectionIn RO
    SetOutPath $INSTDIR
    File /r arcvideo-editor\*
    WriteUninstaller "$INSTDIR\uninstall.exe"

    # Install Visual C++ 2010 Redistributable
    #File "vcredist_x64.exe"
    #ExecWait '"$INSTDIR\vcredist_x64.exe" /quiet'
    #Delete "$INSTDIR\vcredist_x64.exe"
SectionEnd

Section "Create Desktop shortcut"
    CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_TARGET}.exe"
SectionEnd

Section "Create Start Menu shortcut"
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${APP_TARGET}.exe"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

Section "Associate *.ove files with ArcVideo"
    WriteRegStr HKCR ".ove" "" "ArcVideoEditor.OVEFile"
    WriteRegStr HKCR ".ove" "Content Type" "application/vnd.arcvideo-project"
    WriteRegStr HKCR "ArcVideoEditor.OVEFile" "" "ArcVideo project file"
    WriteRegStr HKCR "ArcVideoEditor.OVEFile\DefaultIcon" "" "$INSTDIR\arcvideo-editor.exe,1"
    WriteRegStr HKCR "ArcVideoEditor.OVEFile\shell\open\command" "" "$\"$INSTDIR\arcvideo-editor.exe$\" $\"%1$\""
    System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
SectionEnd

UninstPage uninstConfirm
UninstPage instfiles

Section "uninstall"

    rmdir /r "$INSTDIR"

    Delete "$DESKTOP\${APP_NAME}.lnk"
    rmdir /r "$SMPROGRAMS\${APP_NAME}"

    DeleteRegKey HKCR ".ove"
    DeleteRegKey HKCR "ArcVideoEditor.OVEFile"
    DeleteRegKey HKCR "ArcVideoEditor.OVEFile\DefaultIcon" ""
    DeleteRegKey HKCR "ArcVideoEditor.OVEFile\shell\open\command" ""
    System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
SectionEnd

Function LaunchArcVideo
    ShellExecAsUser::ShellExecAsUser "" "$INSTDIR\${APP_TARGET}.exe"
FunctionEnd
