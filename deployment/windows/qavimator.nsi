;NSIS Modern User Interface
;QAvimator Installer Script

;--------------------------------
;Includes

	;Modern UI
	!include "MUI2.nsh"
	;Needed to use ${GetSize}
	!include "FileFunc.nsh"
	
;--------------------------------
;Interface settings

	!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
	!define MUI_FINISHPAGE_NOAUTOCLOSE
	!define MUI_UNFINISHPAGE_NOAUTOCLOSE
	!define MUI_ABORTWARNING
	
;--------------------------------
;Variable definitions

	!define APPLICATION_NAME "QAvimator"

	!define REGISTRY_ROOTKEY HKCU
	!define REGISTRY_INSTALLKEY "Software\${APPLICATION_NAME}"
	!define REGISTRY_UNINSTALLKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}"
	
	Var StartMenuFolder
	
;--------------------------------
;General
	
	Name "${APPLICATION_NAME}"
	OutFile "QAvimatorTest.exe"
	InstallDir "$PROGRAMFILES\${APPLICATION_NAME}"
	InstallDirRegKey ${REGISTRY_ROOTKEY} "${REGISTRY_INSTALLKEY}" ""
	RequestExecutionLevel admin

;--------------------------------
;Pages
	
	;Installer pages
	!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of ${APPLICATION_NAME}. \r\n \r\n Click Next to continue."
	!insertmacro MUI_PAGE_WELCOME
	
	!insertmacro MUI_PAGE_LICENSE ..\COPYING
	
	!insertmacro MUI_PAGE_DIRECTORY
	
	!insertmacro MUI_PAGE_STARTMENU "${APPLICATION_NAME}" $StartMenuFolder
	
	!insertmacro MUI_PAGE_INSTFILES
	
	!define MUI_FINISHPAGE_RUN qavimator.exe
	!define MUI_FINISHPAGE_RUN_TEXT "Run ${APPLICATION_NAME}"
	!define MUI_FINISHPAGE_LINK qavimator.org

	!insertmacro MUI_PAGE_FINISH
	
	;Uninstaller pages
	!insertmacro MUI_UNPAGE_CONFIRM
	
	!insertmacro MUI_UNPAGE_INSTFILES
	!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

	!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer section

Section "Install"

	SetOutPath "$INSTDIR"
	
	File qavimator.exe
	
	SetOutPath "$INSTDIR\data"
	
	File data\Relaxed.avm
	File data\Relaxed.bvh
	File data\Relaxed_2.bvh
	File data\SL.lim
	File data\SLFemale.bvh
	File data\SLMale.bvh
	File data\TPose.avm
	File data\TPose.bvh
	
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_INSTALLKEY}" "" "$INSTDIR"
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"DisplayName" "${APPLICATION_NAME}"
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"DisplayIcon" "$\"$INSTDIR\qavimator.exe$\""
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"URLInfoAbout" "qavimator.org"
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"DisplayVersion" "0.2013.9.2"
	WriteRegDWORD ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"NoModify" 1
	WriteRegDWORD ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"NoRepair" 1
		
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"EstimatedSize" "$0"
	
	!insertmacro MUI_STARTMENU_WRITE_BEGIN "${APPLICATION_NAME}"
		CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\QAvimator.lnk" "$INSTDIR\QAvimator.exe"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END
	
	
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	
SectionEnd

;--------------------------------
;Uninstaller section

Section "Uninstall"
	
	DeleteRegKey ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}"
	DeleteRegKey ${REGISTRY_ROOTKEY} "${REGISTRY_INSTALLKEY}"
	
	;TODO Delete explicitly and warn user of unexpected files
	RMDir /r "$INSTDIR"
	
	!insertmacro MUI_STARTMENU_GETFOLDER "${APPLICATION_NAME}" $StartMenuFolder
	
	;TODO Delete explicitly and warn user of unexpected files
	RMDir /r "$SMPROGRAMS\$StartMenuFolder"
	
SectionEnd
