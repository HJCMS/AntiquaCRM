;; WARNING: ASCII with CRLF line terminators

;; Project main settings
#define MY_APPL_NAME "@PROJECTNAME@"
#define MY_APPL_UUID "@ANTIQUACRM_APPID@"
#define MY_APPL_DISPLAY_NAME "@DISPLAYNAME@"
#define MY_APPL_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#define MY_APPL_PUBLISHER "HJCMS"
#define MY_APPL_HOMEPAGE "@HOMEPAGEURL@"
#define MY_APPL_FAQ_URL "https://github.com/HJCMS/AntiquaCRM"
#define MY_APPL_BINARY "@PROJECTNAME@.exe"
#define MY_APPL_DOMAIN "hjcms.de"

;; Release date and times
#define CURRENT_YEAR GetDateTimeString('yyyy', '', '')
#define MY_APPL_PKG_RELEASE GetDateTimeString('yyyy-mm-dd-hhMM', '', '')

;; Path settings
#define BUILD_DIR "@ANTIQUACRM_CURRENT_BINARY_DIR@"
#define SOURCE_DIR "@ANTIQUACRM_CURRENT_SOURCE_DIR@"
#define TPL_DIR "@ANTIQUACRM_CURRENT_SOURCE_DIR@\xdg"
#define QT5_PATH "E:\Development\qt\5.15.2\mingw81_64"
#define QRENCODE_PATH "E:\Development\qt\5.15.2\mingw81_64"
#define PgSQL_PATH "E:\Development\PostgreSQL\bin"
#define CURL_PATH "E:\Development\cUrl\bin"
;; Special copy target
#define COPY_DIR "..."
#define CURL_HOME "..."

[Setup]
AppId={#MY_APPL_UUID}
AppName={#MY_APPL_DISPLAY_NAME}
AppVersion={#MY_APPL_VERSION}
AppVerName={#MY_APPL_NAME} {#MY_APPL_VERSION}
AppPublisher={#MY_APPL_PUBLISHER}
AppPublisherURL={#MY_APPL_HOMEPAGE}
AppSupportURL={#MY_APPL_FAQ_URL}
AppUpdatesURL={#MY_APPL_FAQ_URL}
AppReadmeFile="{#MY_APPL_FAQ_URL}/blob/main/README.md"
AppCopyright="Copyright(C) 2011-{#CURRENT_YEAR} {#MY_APPL_PUBLISHER} {#MY_APPL_HOMEPAGE}"
AppContact="{#MY_APPL_PUBLISHER} {#MY_APPL_HOMEPAGE}"
AppComments="Antiquarisches Datenbank Verwaltungsprogramm"
DefaultGroupName={#MY_APPL_DISPLAY_NAME}
DefaultDirName={commonpf64}\{#MY_APPL_PUBLISHER}\{#MY_APPL_NAME}
DisableDirPage=yes
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
OutputBaseFilename={#MY_APPL_NAME}-{#MY_APPL_VERSION}-{#MY_APPL_PKG_RELEASE}-x86_64-installer
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
BeveledLabel=Copyright(C) 2011-{#CURRENT_YEAR} {#MY_APPL_PUBLISHER} {#MY_APPL_HOMEPAGE}

[Tasks]
Name: "application"; Description: "{#MY_APPL_DISPLAY_NAME} {cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: "{app}"; Attribs: system;
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
Source: "{#BUILD_DIR}\src\antiquacrm.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#TPL_DIR}\antiquacrm.ico"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\assistant\antiquacrm_assistant.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\lib\libAntiquaCRM.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#TPL_DIR}\qt.conf"; DestDir: "{app}"; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\i18n\antiquacrm_de.qm"; DestDir: "{app}\i18n"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\pgsql\*.sql"; DestDir: "{app}\data\pgsql\"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\json\*.json"; DestDir: "{app}\data\json\"; Attribs: system; Flags: ignoreversion;
Source: "{#SOURCE_DIR}\src\data\fonts\*.ttf"; DestDir: "{app}\data\fonts"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\libAntiquaCRMPlugin.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\abebooks\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\buchfreund\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\booklooker\lib*.dll"; DestDir: "{app}\plugins\antiquacrm\provider"; Attribs: system; Flags: ignoreversion;
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
Name: "{group}\{#MY_APPL_DISPLAY_NAME}"; Filename: "{app}\{#MY_APPL_BINARY}"; IconFilename: "{app}\{#MY_APPL_NAME}.ico";
Name: "{group}\{cm:UninstallProgram,{#MY_APPL_NAME}}"; Filename: "{uninstallexe}"; IconFilename: "{app}\{#MY_APPL_NAME}.ico";
Name: "{commondesktop}\{#MY_APPL_DISPLAY_NAME}"; Filename: "{app}\{#MY_APPL_BINARY}"; Tasks: "application"; IconFilename: "{app}\{#MY_APPL_NAME}.ico";

[Registry]
;; HKEY_LOCAL_MACHINE
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_BINARY}";
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_BINARY}"; ValueType: string; ValueName: "Version"; ValueData: "{#MY_APPL_VERSION}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_BINARY}"; ValueType: string; ValueName: "Author"; ValueData: "{#MY_APPL_PUBLISHER}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_BINARY}"; ValueType: string; ValueName: "Homepage"; ValueData: "{#MY_APPL_HOMEPAGE}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_BINARY}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{commonpf64}\HJCMS\{#MY_APPL_BINARY}"; Flags: uninsdeletekey;

;; EOF

