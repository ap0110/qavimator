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
	!define FILE_NAME "${APPLICATION_NAME}_${APPLICATION_VERSION}.${APPLICATION_BUILD_NUMBER}_Windows_Setup.exe"

	!define WEBSITE "qavimator.org"
	!define ORGANIZATION "QAvimator Team"

	!define ENGLISH_US_CODE 1033

	!define REGISTRY_ROOT_KEY HKCU
	!define REGISTRY_INSTALL_KEY "Software\${APPLICATION_NAME}"
	!define REGISTRY_UNINSTALL_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPLICATION_NAME}"

	Var StartMenuFolder

;--------------------------------
;Interface settings

	!define MUI_ICON "${RESOURCE_DIR}\qavimator.ico"
	!define MUI_FINISHPAGE_NOAUTOCLOSE
	!define MUI_UNFINISHPAGE_NOAUTOCLOSE
	!define MUI_ABORTWARNING

;--------------------------------
;General

	SetCompressor /SOLID /FINAL zlib

	Name "${APPLICATION_NAME}"
	OutFile "${FILE_NAME}"
	InstallDir "$PROGRAMFILES\${APPLICATION_NAME}"

	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "FileDescription" "QAvimator Windows installer"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "FileVersion" "${APPLICATION_VERSION}.0"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "ProductName" "${APPLICATION_NAME}"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "ProductVersion" "${APPLICATION_VERSION}"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "CompanyName" "${ORGANIZATION}"
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "LegalCopyright" ""
	VIAddVersionKey /LANG=${ENGLISH_US_CODE} "OriginalFilename" "${FILE_NAME}"
	VIProductVersion "${APPLICATION_VERSION}.0"

	RequestExecutionLevel admin

;--------------------------------
;Pages

	;Installer pages
	!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of ${APPLICATION_NAME}. $\r$\n $\r$\n Click Next to continue."
	!insertmacro MUI_PAGE_WELCOME

	!insertmacro MUI_PAGE_LICENSE "${PROJECT_ROOT_DIR}\COPYING"

	!insertmacro MUI_PAGE_DIRECTORY

	!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REGISTRY_ROOT_KEY}"
	!define MUI_STARTMENUPAGE_REGISTRY_KEY "${REGISTRY_INSTALL_KEY}"
	!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"

	!insertmacro MUI_PAGE_STARTMENU "${APPLICATION_NAME}" $StartMenuFolder

	!insertmacro MUI_PAGE_INSTFILES

	!define MUI_FINISHPAGE_RUN "$INSTDIR\qavimator.exe"
	!define MUI_FINISHPAGE_RUN_TEXT "Run ${APPLICATION_NAME}"
	!define MUI_FINISHPAGE_LINK_LOCATION "${WEBSITE}"

	!insertmacro MUI_PAGE_FINISH

	;Uninstaller pages
	!insertmacro MUI_UNPAGE_CONFIRM

	!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

	!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Function run on initialization of installer

Var OldInstallationDir
Var OldStartMenuFolder
Var OldVersion

Function .onInit

	;If the user did not uninstall the previous version, then
	; ask before running its uninstaller
	ReadRegStr $OldInstallationDir ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}" ""
	StrCmp "$OldInstallationDir" "" continueInstallation

	MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2 \
		"A version of QAvimator is already installed! \
		$\nThis version must be removed before installation can continue. \
		$\n$\nRemove current installation of QAvimator?" \
		/SD IDCANCEL \
		IDOK runUninstaller
		Abort

	runUninstaller:

	;Get the "Start Menu Folder" name in case of version 20100106
	ReadRegStr $OldStartMenuFolder ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}" "Start Menu Folder"
	ReadRegStr $OldVersion ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}" "Version"

	;Run the installer and wait for it to return
	ClearErrors
	ExecWait '"$OldInstallationDir\uninstall.exe" _?=$OldInstallationDir'

	IfErrors abortInstallation

	;Test if the uninstaller ran by re-checking the registry
	ReadRegStr $R0 ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}" ""
	;If registry value still exists, then abort, otherwise remove uninstaller
	StrCmp "$R0" "" 0 abortInstallation

	;Remove the uninstaller
	Delete "$OldInstallationDir\uninstall.exe"
	RMDir "$OldInstallationDir"

	;If "Start Menu Folder" was in registry...
	StrCmp "$OldStartMenuFolder" "" continueInstallation 0
	;...and "Version" was 20100106, then 20100106 was previously installed
	StrCmp "$OldVersion" "20100106" 0 continueInstallation

	;20100106 installer incorrectly leaves a shortcut folder in
	; the "All Users" Start Menu, so explicitly delete it
	SetShellVarContext all

	Delete "$SMPROGRAMS\$OldStartMenuFolder\QAvimator.lnk"
	Delete "$SMPROGRAMS\$OldStartMenuFolder\Uninstall.lnk"
	RMDir "$SMPROGRAMS\$OldStartMenuFolder"

	SetShellVarContext current

	Goto continueInstallation

	abortInstallation:

	Abort

	continueInstallation:

FunctionEnd

;--------------------------------
;Installer section

Section "Install"

	SetOutPath "$INSTDIR"

	File "${PROJECT_ROOT_DIR}\_install\qavimator.exe"
	File "${PROJECT_ROOT_DIR}\_install\NEWS"

	SetOutPath "$INSTDIR\data"

	File "${PROJECT_ROOT_DIR}\_install\data\cone.obj"
	File "${PROJECT_ROOT_DIR}\_install\data\cube.obj"
	File "${PROJECT_ROOT_DIR}\_install\data\Relaxed.avm"
	File "${PROJECT_ROOT_DIR}\_install\data\Relaxed.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\Relaxed_2.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\SL.lim"
	File "${PROJECT_ROOT_DIR}\_install\data\SLFemale.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\SLMale.bvh"
	File "${PROJECT_ROOT_DIR}\_install\data\sphere.obj"
	File "${PROJECT_ROOT_DIR}\_install\data\torus.obj"
	File "${PROJECT_ROOT_DIR}\_install\data\TPose.avm"
	File "${PROJECT_ROOT_DIR}\_install\data\TPose.bvh"
	
	!ifdef QT_DIR
		SetOutPath "$INSTDIR"
	
		File "${QT_DIR}\bin\icudt52.dll"
		File "${QT_DIR}\bin\icuin52.dll"
		File "${QT_DIR}\bin\icuuc52.dll"
		File "${QT_DIR}\bin\libgcc_s_dw2-1.dll"
		File "${QT_DIR}\bin\libstdc++-6.dll"
		File "${QT_DIR}\bin\libwinpthread-1.dll"
		File "${QT_DIR}\bin\Qt5Core.dll"
		File "${QT_DIR}\bin\Qt5Gui.dll"
		File "${QT_DIR}\bin\Qt5Network.dll"
		File "${QT_DIR}\bin\Qt5OpenGL.dll"
		File "${QT_DIR}\bin\Qt5Widgets.dll"
		File "${RESOURCE_DIR}\qt.conf"
		
		SetOutPath "$INSTDIR\plugins\platforms"
		
		File "${QT_DIR}\plugins\platforms\qwindows.dll"
	!endif

	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}" "" "$INSTDIR"
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}" \
		"Version" "${APPLICATION_VERSION}"

	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"DisplayName" "${APPLICATION_NAME}"
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"DisplayIcon" "$\"$INSTDIR\qavimator.exe$\""
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"URLInfoAbout" "${WEBSITE}"
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"Publisher" "${ORGANIZATION}"
	WriteRegStr ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"DisplayVersion" "${APPLICATION_VERSION}"
	WriteRegDWORD ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"NoModify" 1
	WriteRegDWORD ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"NoRepair" 1

	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}" \
		"EstimatedSize" "$0"

	SetOutPath "$INSTDIR"
	!insertmacro MUI_STARTMENU_WRITE_BEGIN "${APPLICATION_NAME}"
		;20100106 installer incorrectly leaves a shortcut folder in
		; the "All Users" Start Menu. If it was already uninstalled, then the
		; previous start menu folder is unknown. However, it could be the
		; current start menu folder, so try to delete the shortcuts anyway.
		SetShellVarContext all
		Delete "$SMPROGRAMS\$StartMenuFolder\QAvimator.lnk"
		Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
		RMDir "$SMPROGRAMS\$StartMenuFolder"

		SetShellVarContext current
		CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\QAvimator.lnk" "$INSTDIR\qavimator.exe"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END


	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller section

Section "Uninstall"

	Delete "$INSTDIR\data\cone.obj"
	Delete "$INSTDIR\data\cube.obj"
	Delete "$INSTDIR\data\Relaxed.avm"
	Delete "$INSTDIR\data\Relaxed.bvh"
	Delete "$INSTDIR\data\Relaxed_2.bvh"
	Delete "$INSTDIR\data\SL.lim"
	Delete "$INSTDIR\data\SLFemale.bvh"
	Delete "$INSTDIR\data\SLMale.bvh"
	Delete "$INSTDIR\data\sphere.obj"
	Delete "$INSTDIR\data\torus.obj"
	Delete "$INSTDIR\data\TPose.avm"
	Delete "$INSTDIR\data\TPose.bvh"
	RMDir "$INSTDIR\data"
	
	!ifdef QT_DIR
		Delete "$INSTDIR\plugins\platforms\qwindows.dll"
		RMDir "$INSTDIR\plugins\platforms"
		RMDir "$INSTDIR\plugins"
	
		Delete "$INSTDIR\icudt52.dll"
		Delete "$INSTDIR\icuin52.dll"
		Delete "$INSTDIR\icuuc52.dll"
		Delete "$INSTDIR\libgcc_s_dw2-1.dll"
		Delete "$INSTDIR\libstdc++-6.dll"
		Delete "$INSTDIR\libwinpthread-1.dll"
		Delete "$INSTDIR\Qt5Core.dll"
		Delete "$INSTDIR\Qt5Gui.dll"
		Delete "$INSTDIR\Qt5Network.dll"
		Delete "$INSTDIR\Qt5OpenGL.dll"
		Delete "$INSTDIR\Qt5Widgets.dll"
		Delete "$INSTDIR\qt.conf"
	!endif
	
	Delete "$INSTDIR\NEWS"
	Delete "$INSTDIR\qavimator.exe"
	Delete "$INSTDIR\uninstall.exe"
	RMDir "$INSTDIR"

	!insertmacro MUI_STARTMENU_GETFOLDER "${APPLICATION_NAME}" $StartMenuFolder

	Delete "$SMPROGRAMS\$StartMenuFolder\QAvimator.lnk"
	Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
	RMDir "$SMPROGRAMS\$StartMenuFolder"

	DeleteRegKey ${REGISTRY_ROOT_KEY} "${REGISTRY_INSTALL_KEY}"
	DeleteRegKey ${REGISTRY_ROOT_KEY} "${REGISTRY_UNINSTALL_KEY}"

SectionEnd
