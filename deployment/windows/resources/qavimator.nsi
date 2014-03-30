;NSIS Modern User Interface
;QAvimator Installer Script

;--------------------------------
;Includes

	;Modern UI
	!include "MUI2.nsh"
	;Needed to use ${GetSize}
	!include "FileFunc.nsh"
	
;--------------------------------
;Variable definitions

	;Directory containing the program and data files should be set from command line
	!ifndef PROJECT_ROOT_DIR
		!define PROJECT_ROOT_DIR "."
	!endif
	
	;Directory containing extra files needed for the installer should be set from command line
	!ifndef RESOURCE_DIR
		!define RESOURCE_DIR "."
	!endif
	
	;Application name and version should be set from command line
	!ifndef APPLICATION_NAME | APPLICATION_VERSION | APPLICATION_BUILD_NUMBER
		!define APPLICATION_NAME "QAvimator-Development"
		!define APPLICATION_VERSION "0.0.0"
		!define APPLICATION_BUILD_NUMBER "0"
	!endif
	!define FILE_NAME "${APPLICATION_NAME}_${APPLICATION_VERSION}_${APPLICATION_BUILD_NUMBER}_win32.exe"
	
	!define WEBSITE "qavimator.org"
	
	!define ENGLISH_US_CODE 1033
	
	!define REGISTRY_ROOTKEY HKCU
	!define REGISTRY_INSTALLKEY "Software\${APPLICATION_NAME}"
	!define REGISTRY_UNINSTALLKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}"
	
	Var StartMenuFolder
	
;--------------------------------
;Interface settings

	!define MUI_ICON "${RESOURCE_DIR}\qavimator.ico"
	!define MUI_FINISHPAGE_NOAUTOCLOSE
	!define MUI_UNFINISHPAGE_NOAUTOCLOSE
	!define MUI_ABORTWARNING
	
;--------------------------------
;General
	
	SetCompressor /SOLID /FINAL lzma
	SetCompressorDictSize 32
	
	Name "${APPLICATION_NAME}"
	OutFile "${FILE_NAME}"
	InstallDir "$PROGRAMFILES\${APPLICATION_NAME}"
	InstallDirRegKey ${REGISTRY_ROOTKEY} "${REGISTRY_INSTALLKEY}" ""
	
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "FileDescription" "QAvimator Windows installer"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "FileVersion" "${APPLICATION_VERSION}.0"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "ProductName" "${APPLICATION_NAME}"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "ProductVersion" "${APPLICATION_VERSION}"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "CompanyName" "QAvimator Team"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "LegalCopyright" ""
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "OriginalFilename" "${FILE_NAME}"
	VIProductVersion "${APPLICATION_VERSION}.0"
	
	RequestExecutionLevel admin

;--------------------------------
;Pages
	
	;Installer pages
	!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of ${APPLICATION_NAME}. \r\n \r\n Click Next to continue."
	!insertmacro MUI_PAGE_WELCOME
	
	!insertmacro MUI_PAGE_LICENSE "${PROJECT_ROOT_DIR}\COPYING"
	
	!insertmacro MUI_PAGE_DIRECTORY
	
	!insertmacro MUI_PAGE_STARTMENU "${APPLICATION_NAME}" $StartMenuFolder
	
	!insertmacro MUI_PAGE_INSTFILES
	
	!define MUI_FINISHPAGE_RUN "$INSTDIR\qavimator.exe"
	!define MUI_FINISHPAGE_RUN_TEXT "Run ${APPLICATION_NAME}"
	!define MUI_FINISHPAGE_LINK_LOCATION "${WEBSITE}"

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
	
	File "${PROJECT_ROOT_DIR}\_install\qavimator.exe"
	File "${PROJECT_ROOT_DIR}\_install\glut32.dll"
	!ifdef QT_BIN
		File "${QT_BIN}\icudt51.dll"
		File "${QT_BIN}\icuin51.dll"
		File "${QT_BIN}\icuuc51.dll"
		File "${QT_BIN}\libgcc_s_dw2-1.dll"
		File "${QT_BIN}\libstdc++-6.dll"
		File "${QT_BIN}\libwinpthread-1.dll"
		File "${QT_BIN}\Qt5Core.dll"
		File "${QT_BIN}\Qt5Gui.dll"
		File "${QT_BIN}\Qt5Network.dll"
		File "${QT_BIN}\Qt5OpenGL.dll"
		File "${QT_BIN}\Qt5Widgets.dll"
	!endif
	
	SetOutPath "$INSTDIR\data"
	
	File "${PROJECT_ROOT_DIR}\_install\data\Relaxed.avm"
	File "${PROJECT_ROOT_DIR}\_install\data\Relaxed.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\Relaxed_2.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\SL.lim"
	File "${PROJECT_ROOT_DIR}\_install\data\SLFemale.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\SLMale.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\TPose.avm"
	File "${PROJECT_ROOT_DIR}\_install\data\TPose.bvh"
	
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_INSTALLKEY}" "" "$INSTDIR"
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"DisplayName" "${APPLICATION_NAME}"
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"DisplayIcon" "$\"$INSTDIR\qavimator.exe$\""
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"URLInfoAbout" "${WEBSITE}"
	WriteRegStr ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}" \
		"DisplayVersion" "${APPLICATION_VERSION}"
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
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\QAvimator.lnk" "$INSTDIR\qavimator.exe"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END
	
	
	WriteUninstaller "$INSTDIR\uninstall.exe"
	
SectionEnd

;--------------------------------
;Uninstaller section

Section "Uninstall"
	
	Delete "$INSTDIR\data\Relaxed.avm"
	Delete "$INSTDIR\data\Relaxed.bvh"
	Delete "$INSTDIR\data\Relaxed_2.bvh"
	Delete "$INSTDIR\data\SL.lim"
	Delete "$INSTDIR\data\SLFemale.bvh"
	Delete "$INSTDIR\data\SLMale.bvh"
	Delete "$INSTDIR\data\TPose.avm"
	Delete "$INSTDIR\data\TPose.bvh"
	RMDir "$INSTDIR\data"
	
	Delete "$INSTDIR\glut32.dll"
	!ifdef QT_BIN
		Delete "$INSTDIR\icudt51.dll"
		Delete "$INSTDIR\icuin51.dll"
		Delete "$INSTDIR\icuuc51.dll"
		Delete "$INSTDIR\libgcc_s_dw2-1.dll"
		Delete "$INSTDIR\libstdc++-6.dll"
		Delete "$INSTDIR\libwinpthread-1.dll"
		Delete "$INSTDIR\Qt5Core.dll"
		Delete "$INSTDIR\Qt5Gui.dll"
		Delete "$INSTDIR\Qt5Network.dll"
		Delete "$INSTDIR\Qt5OpenGL.dll"
		Delete "$INSTDIR\Qt5Widgets.dll"
	!endif
	Delete "$INSTDIR\qavimator.exe"
	Delete "$INSTDIR\uninstall.exe"
	RMDir "$INSTDIR"
	
	!insertmacro MUI_STARTMENU_GETFOLDER "${APPLICATION_NAME}" $StartMenuFolder
	
	Delete "$SMPROGRAMS\$StartMenuFolder\QAvimator.lnk"
	Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
	RMDir "$SMPROGRAMS\$StartMenuFolder"
	
	DeleteRegKey ${REGISTRY_ROOTKEY} "${REGISTRY_INSTALLKEY}"
	DeleteRegKey ${REGISTRY_ROOTKEY} "${REGISTRY_UNINSTALLKEY}"
	
SectionEnd
