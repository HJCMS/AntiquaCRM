; -*- coding: utf-8 -*-
; vim: set fileencoding=utf-8

#define MyAppName "@PROJECTNAME@"
#define MyAppDisplayName "@DISPLAYNAME@"
#define MyAppVersion "@ANTIQUACRM_VERSION_STRING@"
#define MyAppPublisher "HJCMS"
#define MyAppURL "https://www.hjcms.de"
#define MyAppExeName "@PROJECTNAME@.exe"
#define MyDomainName "hjcms.de"
#define QT5_PATH "F:\Development\qt\5.15.2\mingw81_64\bin"
#define PgSQL_PATH "F:\Development\PostgreSQL\bin"
#define BUID_DIR "F:\Development\antiqua\build"

[Setup]
AppId=
AppName={#MyAppDisplayName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}/index.html
AppSupportURL={#MyAppURL}/contakt.html
AppUpdatesURL={#MyAppURL}
AppCopyright="Copyright(C) 2011-2022 {#MyAppPublisher} {#MyAppURL}"
AppComments="Antiquarisches Datenbank Verwaltungsprogramm"
DefaultDirName={pf}\{#MyAppPublisher}\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
LicenseFile=COPYING
OutputDir=.
SetupIconFile=.\xdg\antiqua.ico
Compression=lzma
SolidCompression=yes
CreateUninstallRegKey=yes
PrivilegesRequired=admin
Uninstallable=yes
UserInfoPage=no
DirExistsWarning=yes
UsePreviousAppDir=yes
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-x86_64-installer
;; Window Style
WindowVisible=yes
WindowShowCaption=no
WizardImageStretch=yes  
WizardImageFile=.\xdg\antiqua-label.bmp
WizardImageBackColor=clAqua

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[CustomMessages]
english.NormalInst=Default Application Data
german.NormalInst=Standard Programm Dateien
english.DevelInst=Development Headers and Library
german.DevelInst=Entwickler Bibliotheken und Include Dateien.

[Messages]
BeveledLabel=Copyright(C) 2011-2022 {#MyAppPublisher} {#MyAppURL}

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1


[Dirs]
Name: "{app}\plugins"; Attribs: system;
Name: "{app}\plugins\imageformats"; Attribs: system;
Name: "{app}\plugins\platforms"; Attribs: system;
Name: "{app}\plugins\printsupport"; Attribs: system;
Name: "{app}\plugins\sqldrivers"; Attribs: system;

[Files]  
Source: "{#BUID_DIR}\src\antiquacrm.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUID_DIR}\xdg\antiquacrm.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUID_DIR}\xdg\qt.conf"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\src\i18n\antiquacrm_de.qm"; DestDir: "{app}"; Flags: ignoreversion
;; MinGW
;; Source: "{#QT5_PATH}\gpsvc.dll"; DestDir: "{app}"; Flags: ignoreversion
;; Source: "{#QT5_PATH}\IEShims.dll"; DestDir: "{app}"; Flags: ignoreversion
;; Source: "{#QT5_PATH}\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
;; Source: "{#QT5_PATH}\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\libwinpthread-1".dll; DestDir: "{app}"; Flags: ignoreversion
;; PostgreSQL
Source: "{#PgSQL_PATH}\libcrypto-1_1-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libiconv-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libintl-9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\liblz4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libpq.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libssl-1_1-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
;; Qt5
Source: "{#QT5_PATH}\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\Qt5Xml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\plugins\imageformats\q*.dll"; DestDir: "{app}\plugins\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\platforms\qwindows.dll"; DestDir: "{app}\plugins\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\printsupport\windowsprintersupport.dll"; DestDir: "{app}\plugins\printsupport"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\sqldrivers\qsqlpsql.dll"; DestDir: "{app}\plugins\sqldrivers"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Registry]
;; HKEY_LOCAL_MACHINE
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}";
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Version"; ValueData: "{#MyAppVersion}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Author"; ValueData: "{#MyAppPublisher}"; Flags: uninsdeletekey; 
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Homepage"; ValueData: "{#MyAppURL}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{pf}\HJCMS\{#MyAppExeName}"; Flags: uninsdeletekey;
;; HKEY_CURRENT_USER
Root: HKCU; Subkey: "Software\{#MyDomainName}\{#MyAppExeName}"; Flags: uninsdeletekey;
Root: HKCU; Subkey: "Software\{#MyDomainName}"; Flags: uninsdeletekeyifempty

;; EOF

