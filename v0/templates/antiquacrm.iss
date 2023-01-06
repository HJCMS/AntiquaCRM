; -*- coding: utf-8 -*-
; vim: set fileencoding=utf-8
;
; TODO - Required Package
; Visual C++ Redistributable für Visual Studio 2015
; https://www.microsoft.com/de-de/download/details.aspx?id=48145
; https://docs.microsoft.com/de-DE/cpp/windows/latest-supported-vc-redist?view=msvc-170
; PermaLink: https://aka.ms/vs/17/release/vc_redist.x64.exe
;
#define MyAppName "antiquacrm"
#define MyAppDisplayName "Antiqua CRM"
#define MyAppVersion "0.5.5"
#define MyAppPublisher "HJCMS"
#define MyAppURL "https://www.hjcms.de"
#define MyAppExeName "antiquacrm.exe"
#define MyDomainName "hjcms.de"
#define QT5_PATH "/home/heinemann/Programme/Qt/5.15.2/gcc_64"
#define QRENCODE_PATH "/home/heinemann/Programme/Qt/5.15.2/gcc_64"
#define PgSQL_PATH "E:\Development\PostgreSQL\bin"
#define CURL_PATH "E:\Development\cUrl\bin"
#define BUID_DIR "/home/heinemann/Developement/antiqua/build-antiquacrm-Desktop_Qt_5_15_2_GCC_64bit-Debug/templates"
#define TPL_DIR "/home/heinemann/Developement/antiqua/v0/templates"
#define COPY_DIR "__TODO__"

[Setup]
AppId=9E3A7398-4493-45AE-AF59-5D5FA84DB2F7
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
LicenseFile={#TPL_DIR}\LICENSE.md
OutputDir=.
Compression=lzma
SolidCompression=yes
CreateUninstallRegKey=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64
SetupIconFile={#TPL_DIR}\antiqua.ico
Uninstallable=yes
UserInfoPage=no
DirExistsWarning=yes
UsePreviousAppDir=yes
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-x86_64-installer
;; Window Style
WindowVisible=yes
WindowShowCaption=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[CustomMessages]
english.NormalInst=Default Application Data
german.NormalInst=Standard Programm Dateien

[Messages]
BeveledLabel=Copyright(C) 2011-2022 {#MyAppPublisher} {#MyAppURL}

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: "{app}\i18n"; Attribs: system;
Name: "{app}\xml"; Attribs: system;
Name: "{app}\icons"; Attribs: system;
Name: "{app}\company"; Attribs: system;
Name: "{app}\plugins"; Attribs: system;
Name: "{app}\plugins\antiqua"; Attribs: system;
Name: "{app}\plugins\imageformats"; Attribs: system;
Name: "{app}\plugins\platforms"; Attribs: system;
Name: "{app}\plugins\printsupport"; Attribs: system;
Name: "{app}\plugins\sqldrivers"; Attribs: system;
Name: "{app}\plugins\generic"; Attribs: system;
Name: "{app}\plugins\imageformats"; Attribs: system;
Name: "{app}\plugins\iconengines"; Attribs: system;
Name: "{app}\plugins\platforms"; Attribs: system;
Name: "{app}\plugins\platformthemes"; Attribs: system;
Name: "{app}\plugins\printsupport"; Attribs: system;
Name: "{app}\plugins\sqldrivers"; Attribs: system;

[Files]
Source: "{#BUID_DIR}\src\antiquacrm.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUID_DIR}\src\core\antiquacore\libAntiquaCore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#TPL_DIR}\qt.conf"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUID_DIR}\src\i18n\antiquacrm.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "{#BUID_DIR}\src\xml\*.xml"; DestDir: "{app}\xml"; Flags: ignoreversion
Source: "{#BUID_DIR}\src\plugins\antiqua\lib*.dll"; DestDir: "{app}\plugins\antiqua"; Flags: ignoreversion
Source: "{#BUID_DIR}\src\icons\*.png"; DestDir: "{app}\icons"; Flags: ignoreversion
;; MinGW
Source: "{#QT5_PATH}\bin\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
;; cUrl
Source: "{#CURL_PATH}\libcurl-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#CURL_PATH}\curl-ca-bundle.crt"; DestDir: "{app}"; Flags: ignoreversion
;; PostgreSQL
Source: "{#PgSQL_PATH}\libcrypto-1_1-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libiconv-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libintl-9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\liblz4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libpq.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libssl-1_1-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PgSQL_PATH}\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
;; QREncode
Source: "{#QRENCODE_PATH}\bin\libzlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QRENCODE_PATH}\bin\libpng16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QRENCODE_PATH}\lib\libqrencode.dll"; DestDir: "{app}"; Flags: ignoreversion
;; Qt5
Source: "{#QT5_PATH}\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5NetworkAuth.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5Xml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\bin\Qt5WinExtras.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT5_PATH}\plugins\generic\*.dll"; DestDir: "{app}\plugins\generic"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\imageformats\*.dll"; DestDir: "{app}\plugins\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\iconengines\*.dll"; DestDir: "{app}\plugins\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\platforms\*.dll"; DestDir: "{app}\plugins\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\platformthemes\*.dll"; DestDir: "{app}\plugins\platformthemes"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\printsupport\*.dll"; DestDir: "{app}\plugins\printsupport"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#QT5_PATH}\plugins\sqldrivers\qsqlpsql.dll"; DestDir: "{app}\plugins\sqldrivers"; Flags: ignoreversion recursesubdirs createallsubdirs
;; Kundendaten
Source: "{#COPY_DIR}\URWChancery_L-Medium-Italic.ttf"; DestDir: "{app}\company"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#COPY_DIR}\druck_header.png"; DestDir: "{app}\company"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#COPY_DIR}\HJCMS-Personal-CA.crt"; DestDir: "{app}\"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppDisplayName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppDisplayName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry]
;; HKEY_LOCAL_MACHINE
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}";
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Version"; ValueData: "{#MyAppVersion}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Author"; ValueData: "{#MyAppPublisher}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Homepage"; ValueData: "{#MyAppURL}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{commonpf}\HJCMS\{#MyAppExeName}"; Flags: uninsdeletekey;

;; EOF

