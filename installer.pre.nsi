﻿Unicode True
!include StrContains.nsh

# 名称
Name "太原机车防火视频"

# 文件名称
Outfile "太原机车防火视频-{version}.exe"
 
# 安装目录
InstallDir "D:"

# default section
Section
 
# define the output path for this file
SetOutPath $INSTDIR
 
# 安装文件
File /r "TaiYuan-Release-{version}"

# 程序启动
SetOutPath "$INSTDIR\TaiYuan-Release-{version}"
CreateShortCut "$SMPROGRAMS\Startup\太原机车防火视频程序.lnk" "$INSTDIR\TaiYuan-Release-{version}\launcher.exe"
SetOutPath "$INSTDIR\TaiYuan-Release-{version}"
CreateShortCut "$SMPROGRAMS\太原机车防火视频程序.lnk" "$INSTDIR\TaiYuan-Release-{version}\launcher.exe"

# 卸载
WriteUninstaller "$INSTDIR\TaiYuan-Release-{version}\uninstaller.exe"
SetOutPath "$INSTDIR\TaiYuan-Release-{version}"
CreateShortCut "$SMPROGRAMS\卸载 太原机车防火视频程序.lnk" "$INSTDIR\TaiYuan-Release-{version}\uninstaller.exe"

sectionend


Section "python安装" SEC2

clearerrors
nsExec::ExecToStack 'cmd /Q /C "python "$INSTDIR\TaiYuan-Release-{version}\platform_info.py"'
Pop $0 ; return value (it always 0 even if an error occured)
Pop $1 ; command output


Push $1
Push "Python"
Call StrContains
Pop $0
StrCmp $0 "" notfound
  Goto done
notfound:
  ExecWait "$INSTDIR\TaiYuan-Release-{version}\python-3.8.10-amd64.exe /quiet InstallAllUsers=1 PrependPath=1 Include_test=1"

done:
  DetailPrint "Found $1"

SectionEnd

section

MessageBox MB_YESNO|MB_ICONQUESTION "安装成功，需要重启计算机。" IDNO +2
Reboot

sectionend


# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
Section "Uninstall"
 
# Always delete uninstaller first
Delete "$INSTDIR\TaiYuan-Release-{version}\uninstaller.exe"

Delete "$SMPROGRAMS\太原机车防火视频程序.lnk"
Delete "$SMPROGRAMS\卸载 太原机车防火视频程序.lnk"
Delete "$SMPROGRAMS\Startup\太原机车防火视频程序.lnk"

# Delete the directory
SetOutPath $INSTDIR
RMDir /r "$INSTDIR\TaiYuan-Release-{version}\"

SectionEnd
