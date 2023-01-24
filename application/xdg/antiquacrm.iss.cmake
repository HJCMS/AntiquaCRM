; -*- coding: utf-8 -*-
; vim: set fileencoding=utf-8

; Project main settings
#define APPL_NAME "@PROJECTNAME@"
#define APPL_UUID "@ANTIQUACRM_APPID@"
#define APPL_DISPLAY_NAME "@DISPLAYNAME@"
#define APPL_VERSION_STRING "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#define APPL_PUBLISHER "HJCMS"
#define APPL_PUBLISHER_URL "@HOMEPAGEURL@"
#define APPL_BINARY "@PROJECTNAME@.exe"
#define APPL_DOMAIN "hjcms.de"
#define APPL_BUILD_YEAR GetDateTimeString('yyyy', '', '')
#define APPL_BUILD_DIR "@ANTIQUACRM_INNOSETUP_BINDIR@"
#define APPL_SOURCE_DIR "@ANTIQUACRM_INNOSETUP_SRCDIR@"
#define APPL_TEMPLATE_DIR "@CMAKE_CURRENT_APPL_SOURCE_DIR@"
#define APPL_PACKAGE_RELEASE GetDateTimeString('yyyymmdd', '', '')

; Qt5 path
#define APPL_QT_PATH "E:\Development\qt\5.15.2\mingw81_64"

; QREncode
#define APPL_QRENCODE_PATH "E:\Development\qt\5.15.2\mingw81_64"

; PostgreSQL
#define APPL_PGSQL_PATH ""

; Personal template files target
#define APPL_COPY_DIR ""

; Required for cUrl CA_BUNDLE PEM File.
; https://curl.se/docs/sslcerts.html
; NOTE Make sure, the destination filename is equal to "curl-ca-bundle.crt"!
#define APPL_CA_BUNDLE ""

[Setup]
AppId={#APPL_UUID}
AppName={#APPL_DISPLAY_NAME}
AppVersion={#APPL_VERSION_STRING}
AppVerName={#APPL_NAME} {#APPL_VERSION_STRING}
AppPublisher={#APPL_PUBLISHER}
AppPublisherURL={#APPL_PUBLISHER_URL}
AppSupportURL={#APPL_PUBLISHER_URL}
AppUpdatesURL={#APPL_PUBLISHER_URL}
AppCopyright="Copyright(C) 2011-{#APPL_BUILD_YEAR} {#APPL_PUBLISHER} {#APPL_PUBLISHER_URL}"
AppContact="{#APPL_PUBLISHER} {#APPL_PUBLISHER_URL}"
AppComments="Antiquarisches Datenbank Verwaltungsprogramm"
DefaultDirName={commonpf64}\{#APPL_PUBLISHER}\{#APPL_NAME}
DisableDirPage=yes
DefaultGroupName={#APPL_NAME}
LicenseFile={#APPL_TEMPLATE_DIR}\LICENSE.md
Compression=lzma
SolidCompression=yes
CreateUninstallRegKey=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64
SetupIconFile={#APPL_TEMPLATE_DIR}\antiquacrm.ico
Uninstallable=yes
UserInfoPage=no
DirExistsWarning=yes
UsePreviousAppDir=yes
WindowVisible=yes
WindowShowCaption=no
; Installer Output
OutputDir=.
OutputBaseFilename={#APPL_NAME}-{#APPL_VERSION_STRING}-x86_64-{#APPL_PACKAGE_RELEASE}-installer

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[CustomMessages]
english.NormalInst=Default Application Data
german.NormalInst=Standard Programm Dateien

[Messages]
BeveledLabel=Copyright(C) 2011-2023 {#APPL_PUBLISHER} {#APPL_PUBLISHER_URL}

[Tasks] 
Name: "application"; Description: "{#APPL_DISPLAY_NAME} {cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

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
Source: "{#APPL_BUILD_DIR}\src\antiquacrm.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\assistant\antiquacrm_assistant.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\lib\libAntiquaCRM.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_TEMPLATE_DIR}\qt.conf"; DestDir: "{app}"; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\i18n\antiquacrm_de.qm"; DestDir: "{app}\i18n"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\data\pgsql\*.sql"; DestDir: "{app}\data\pgsql\"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\data\json\*.json"; DestDir: "{app}\data\json\"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_SOURCE_DIR}\src\data\fonts\*.ttf"; DestDir: "{app}\data\fonts"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\plugins\libAntiquaCRMPlugin.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\plugins\antiquacrm\abebooks\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\plugins\antiquacrm\buchfreund\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_BUILD_DIR}\src\plugins\antiquacrm\booklooker\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#APPL_TEMPLATE_DIR}\*.png"; DestDir: "{app}\icons"; Flags: ignoreversion;
;; MinGW
Source: "{APPL_QT_PATH}\bin\libEGL.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\libgcc_s_seh-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\libstdc++-6.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\libwinpthread-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; PostgreSQL
Source: "{APPL_PGSQL_PATH}\libcrypto-1_1-x64.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\libiconv-2.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\libintl-9.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\liblz4.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\libpq.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\libssl-1_1-x64.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\libwinpthread-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_PGSQL_PATH}\zlib1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; QREncode
Source: "{APPL_QRENCODE_PATH}\bin\libzlib1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QRENCODE_PATH}\bin\libpng16.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QRENCODE_PATH}\lib\libqrencode.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; Qt5
Source: "{APPL_QT_PATH}\bin\Qt5Core.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Gui.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Widgets.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Network.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5NetworkAuth.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Sql.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Svg.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Xml.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5PrintSupport.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5Charts.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\bin\Qt5WinExtras.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "{APPL_QT_PATH}\plugins\generic\*.dll"; DestDir: "{app}\plugins\generic"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_QT_PATH}\plugins\imageformats\*.dll"; DestDir: "{app}\plugins\imageformats"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_QT_PATH}\plugins\iconengines\*.dll"; DestDir: "{app}\plugins\iconengines"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_QT_PATH}\plugins\platforms\*.dll"; DestDir: "{app}\plugins\platforms"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_QT_PATH}\plugins\platformthemes\*.dll"; DestDir: "{app}\plugins\platformthemes"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_QT_PATH}\plugins\printsupport\*.dll"; DestDir: "{app}\plugins\printsupport"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_QT_PATH}\plugins\sqldrivers\qsqlpsql.dll"; DestDir: "{app}\plugins\sqldrivers"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
;; Kundendaten
Source: "{APPL_COPY_DIR}\URWChancery_L-Medium-Italic.ttf"; DestDir: "{app}\company"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_COPY_DIR}\druck_header.png"; DestDir: "{app}\company"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{APPL_COPY_DIR}\Zertifikat\*.crt"; DestDir: "{app}\data\certs\"; Attribs: system; Flags: ignoreversion;
;; Curl CA-Bundle
Source: "{#APPL_CA_BUNDLE}"; DestDir: "{app}\"; Attribs: system; Flags: ignoreversion;

[Icons]
Name: "{group}\{#APPL_DISPLAY_NAME}"; Filename: "{app}\{#APPL_BINARY}";
Name: "{group}\{cm:UninstallProgram,{#APPL_NAME}}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\{#APPL_DISPLAY_NAME}"; Filename: "{app}\{#APPL_BINARY}"; Tasks: "application";

[Registry]
;; HKEY_LOCAL_MACHINE
Root: HKLM; Subkey: "Software\HJCMS\{#APPL_BINARY}";
Root: HKLM; Subkey: "Software\HJCMS\{#APPL_BINARY}"; ValueType: string; ValueName: "Version"; ValueData: "{#APPL_VERSION_STRING}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#APPL_BINARY}"; ValueType: string; ValueName: "Author"; ValueData: "{#APPL_PUBLISHER}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#APPL_BINARY}"; ValueType: string; ValueName: "Homepage"; ValueData: "{#APPL_PUBLISHER_URL}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#APPL_BINARY}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{commonpf64}\HJCMS\{#APPL_BINARY}"; Flags: uninsdeletekey;

;; EOF

