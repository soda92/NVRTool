Unicode True

# define the name of the installer
Outfile "太原机车防火视频-v1.2.2.exe"
 
# define the directory to install to, the desktop in this case as specified  
# by the predefined $DESKTOP variable
InstallDir "D:\TaiYuan-v1.2.2"
 
# default section
Section
 
# define the output path for this file
SetOutPath $INSTDIR
 
# define what to install and place it in the output path
File /r "TaiYuan-Release-v1.2.2"

Delete "$SMPROGRAMS\Startup\LT_*.lnk"
CreateShortCut "$SMPROGRAMS\Startup\TaiYuan-FireVideo.lnk" "$INSTDIR\LT_LCWB-1A.exe"

ExecWait "wusa.exe $INSTDIR\Windows6.1-KB3126587-x64.msu /quiet /norestart"

MessageBox MB_YESNO|MB_ICONQUESTION "安装成功，需要重启计算机。" IDNO +2
Reboot

SectionEnd
