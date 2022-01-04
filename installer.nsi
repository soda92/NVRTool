Unicode True
!include LogicLib.nsh
!include StrContains.nsh

; The name of the installer
Name "太原机车防火视频"

# define the name of the installer
Outfile "太原机车防火视频-v1.2.2.exe"
 
# define the directory to install to, the desktop in this case as specified  
# by the predefined $DESKTOP variable
InstallDir "D:"

# default section
Section
 
# define the output path for this file
SetOutPath $INSTDIR
 
# define what to install and place it in the output path
File /r "TaiYuan-Release-v1.2.2"

Delete "$SMPROGRAMS\Startup\LT_*.lnk"
SetOutPath "$INSTDIR\TaiYuan-Release-v1.2.2"
CreateShortCut "$SMPROGRAMS\Startup\太原机车防火视频程序.lnk" "$INSTDIR\TaiYuan-Release-v1.2.2\launcher.exe"
SetOutPath "$INSTDIR\TaiYuan-Release-v1.2.2"
CreateShortCut "$SMPROGRAMS\太原机车防火视频程序.lnk" "$INSTDIR\TaiYuan-Release-v1.2.2\launcher.exe"

# define uninstaller name
WriteUninstaller "$INSTDIR\TaiYuan-Release-v1.2.2\uninstaller.exe"
SetOutPath "$INSTDIR\TaiYuan-Release-v1.2.2"
CreateShortCut "$SMPROGRAMS\卸载 太原机车防火视频程序.lnk" "$INSTDIR\TaiYuan-Release-v1.2.2\uninstaller.exe"

sectionend

section

clearerrors
nsExec::ExecToStack 'cmd /Q /C "wmic.exe qfe get hotfixid | findstr.exe "^KB3126587""'
Pop $0 ; return value (it always 0 even if an error occured)
Pop $1 ; command output
detailprint $0
detailprint $1

Push $1
Push "KB3126587"
Call StrContains
Pop $0
StrCmp $0 "" notfound
  Goto done
notfound:
    ExecWait "wusa.exe $INSTDIR\TaiYuan-Release-v1.2.2\Windows6.1-KB3126587-x64.msu /quiet /norestart"

done:

sectionend

section

MessageBox MB_YESNO|MB_ICONQUESTION "安装成功，需要重启计算机。" IDNO +2
Reboot

sectionend


# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
Section "Uninstall"
 
# Always delete uninstaller first
Delete "$INSTDIR\TaiYuan-Release-v1.2.2\uninstaller.exe"

Delete "$SMPROGRAMS\太原机车防火视频程序.lnk"
Delete "$SMPROGRAMS\卸载 太原机车防火视频程序.lnk"
Delete "$SMPROGRAMS\Startup\太原机车防火视频程序.lnk"

# Delete the directory
SetOutPath $INSTDIR
RMDir /r "$INSTDIR\TaiYuan-Release-v1.2.2\"

SectionEnd
