; -*- coding: utf-8 -*-
; vim: set fileencoding=utf-8
;
; TODO - Required Package
; Visual C++ Redistributable für Visual Studio 2015
; https://www.microsoft.com/de-de/download/details.aspx?id=48145
; https://docs.microsoft.com/de-DE/cpp/windows/latest-supported-vc-redist?view=msvc-170
; PermaLink: https://aka.ms/vs/17/release/vc_redist.x64.exe
;
#define MyAppName "@PROJECTNAME@"
#define MyAppDisplayName "@DISPLAYNAME@"
#define MyAppVersion "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#define MyAppPublisher "HJCMS"
#define MyAppURL "@HOMEPAGEURL@"
#define MyAppExeName "@PROJECTNAME@.exe"
#define MyDomainName "hjcms.de"
#define QT5_PATH "E:\Development\qt\5.15.2\mingw81_64"
#define QRENCODE_PATH "E:\Development\qt\5.15.2\mingw81_64"
#define PgSQL_PATH "E:\Development\PostgreSQL\bin"
#define CURL_PATH "E:\Development\cUrl\bin"
#define BUID_DIR "@CMAKE_CURRENT_BINARY_DIR@"
#define SOURCE_DIR "@CMAKE_CURRENT_SOURCE_DIR@"
#define TPL_DIR "@CMAKE_CURRENT_SOURCE_DIR@\xdg"
#define COPY_DIR "E:\Development\SomeCopyData"
#define CURL_HOME "E:\Development\cUrl\bin"
#define DateTimeStr GetDateTimeString('yyyy-mm-dd-hhMM', '', '')

[Setup]
AppId=9E3A7398-4493-45AE-AF59-5D5FA84DB2F7
AppName={#MyAppDisplayName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppCopyright="Copyright(C) 2011-2023 {#MyAppPublisher} {#MyAppURL}"
AppContact="{#MyAppPublisher} {#MyAppURL}"
AppComments="Antiquarisches Datenbank Verwaltungsprogramm"
DefaultDirName={commonpf64}\{#MyAppPublisher}\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
LicenseFile={#TPL_DIR}\LICENSE.md
OutputDir=.
Compression=lzma
SolidCompression=yes
CreateUninstallRegKey=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64
SetupIconFile={#TPL_DIR}\antiquacrm.ico
Uninstallable=yes
UserInfoPage=no
DirExistsWarning=yes
UsePreviousAppDir=yes
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-{#DateTimeStr}-x86_64-installer
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
BeveledLabel=Copyright(C) 2011-2023 {#MyAppPublisher} {#MyAppURL}

[Tasks] 
Name: "application"; Description: "{#MyAppDisplayName} {cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: "{app}\i18n"; Attribs: system;
Name: "{app}\data"; Attribs: system;
Name: "{app}\data\pgsql"; Attribs: system;
Name: "{app}\data\json"; Attribs: system; 
Name: "{app}\data\fonts"; Attribs: system;
Name: "{app}\data\certs"; Attribs: system;
Name: "{app}\plugins"; Attribs: system;  
Name: "{app}\plugins\antiquacrm"; Attribs: system;
Name: "{app}\plugins\antiquacrm\provider"; Attribs: system;
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
Source: "{#BUID_DIR}\src\antiquacrm.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\assistant\antiquacrm_assistant.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\lib\libAntiquaCRM.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#TPL_DIR}\qt.conf"; DestDir: "{app}"; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\i18n\antiquacrm_de.qm"; DestDir: "{app}\i18n"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\data\pgsql\*.sql"; DestDir: "{app}\data\pgsql\"; Attribs: system; Flags: ignoreversion; 
Source: "{#BUID_DIR}\src\data\json\*.json"; DestDir: "{app}\data\json\"; Attribs: system; Flags: ignoreversion;
Source: "{#SOURCE_DIR}\src\data\fonts\*.ttf"; DestDir: "{app}\data\fonts"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\plugins\libAntiquaCRMPlugin.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\plugins\antiquacrm\abebooks\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\plugins\antiquacrm\buchfreund\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#BUID_DIR}\src\plugins\antiquacrm\booklooker\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#TPL_DIR}\*.png"; DestDir: "{app}\icons"; Flags: ignoreversion;
;; MinGW
Source: "{#QT5_PATH}\bin\libEGL.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\libgcc_s_seh-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\libstdc++-6.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\libwinpthread-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; PostgreSQL
Source: "{#PgSQL_PATH}\libcrypto-1_1-x64.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\libiconv-2.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\libintl-9.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\liblz4.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\libpq.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\libssl-1_1-x64.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\libwinpthread-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\zlib1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; QREncode
Source: "{#QRENCODE_PATH}\bin\libzlib1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QRENCODE_PATH}\bin\libpng16.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QRENCODE_PATH}\lib\libqrencode.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; Qt5
Source: "{#QT5_PATH}\bin\Qt5Core.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Gui.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Widgets.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Network.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5NetworkAuth.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Sql.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Svg.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Xml.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5PrintSupport.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5Charts.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QT5_PATH}\bin\Qt5WinExtras.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "{#QT5_PATH}\plugins\generic\*.dll"; DestDir: "{app}\plugins\generic"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QT5_PATH}\plugins\imageformats\*.dll"; DestDir: "{app}\plugins\imageformats"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QT5_PATH}\plugins\iconengines\*.dll"; DestDir: "{app}\plugins\iconengines"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QT5_PATH}\plugins\platforms\*.dll"; DestDir: "{app}\plugins\platforms"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QT5_PATH}\plugins\platformthemes\*.dll"; DestDir: "{app}\plugins\platformthemes"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QT5_PATH}\plugins\printsupport\*.dll"; DestDir: "{app}\plugins\printsupport"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QT5_PATH}\plugins\sqldrivers\qsqlpsql.dll"; DestDir: "{app}\plugins\sqldrivers"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
;; Kundendaten
Source: "{#COPY_DIR}\URWChancery_L-Medium-Italic.ttf"; DestDir: "{app}\company"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#COPY_DIR}\druck_header.png"; DestDir: "{app}\company"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#COPY_DIR}\Zertifikat\*.crt"; DestDir: "{app}\data\certs\"; Attribs: system; Flags: ignoreversion;
;; Curl CA-Bundle
Source: "{#CURL_HOME}\curl-ca-bundle.crt"; DestDir: "{app}\"; Attribs: system; Flags: ignoreversion;

[Icons]
Name: "{group}\{#MyAppDisplayName}"; Filename: "{app}\{#MyAppExeName}";
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\{#MyAppDisplayName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: "application";

[Registry]
;; HKEY_LOCAL_MACHINE
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}";
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Version"; ValueData: "{#MyAppVersion}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Author"; ValueData: "{#MyAppPublisher}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "Homepage"; ValueData: "{#MyAppURL}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MyAppExeName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{commonpf64}\HJCMS\{#MyAppExeName}"; Flags: uninsdeletekey;

;; EOF

