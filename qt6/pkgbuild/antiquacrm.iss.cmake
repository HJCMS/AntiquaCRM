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
DefaultDirName={commonpf64}\{#MY_APPL_PUBLISHER}\{#MY_APPL_NAME}\{#MY_APPL_VERSION}
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
Name: "{app}
Name: "{app}\i18n"; Attribs: system;
Name: "{app}\data"; Attribs: system;
Name: "{app}\data\documents"; Attribs: system;
Name: "{app}\data\pgsql"; Attribs: system;
Name: "{app}\data\json"; Attribs: system;
Name: "{app}\data\fonts"; Attribs: system;
Name: "{app}\data\certs"; Attribs: system;
Name: "{app}\plugins"; Attribs: system;
Name: "{app}\plugins\providers"; Attribs: system;
Name: "{app}\plugins\providers/Buchfreund"; Attribs: system;
Name: "{app}\plugins\providers/Booklooker"; Attribs: system;
Name: "{app}\plugins\providers/AbeBooks"; Attribs: system;
Name: "{app}\plugins\acmdproviders"; Attribs: system;
Name: "{app}\plugins\acmdproviders/CmdBuchfreund"; Attribs: system;
Name: "{app}\plugins\acmdproviders/CmdAbeBooks"; Attribs: system;
Name: "{app}\plugins\acmdproviders/CmdBooklooker"; Attribs: system;
Name: "{app}\plugins\tabs"; Attribs: system;
Name: "{app}\plugins\tabs/Reports"; Attribs: system;
Name: "{app}\plugins\tabs/Various"; Attribs: system;
Name: "{app}\plugins\tabs/Statistics"; Attribs: system;
Name: "{app}\plugins\tabs/Books"; Attribs: system;
Name: "{app}\plugins\tabs/Stitches"; Attribs: system;
Name: "{app}\plugins\tabs/CDsVinyl"; Attribs: system;
Name: "{app}\plugins\tabs/Views"; Attribs: system;

[Files]
Source: "{#BUILD_DIR}\src\antiquacrm.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#TPL_DIR}\antiquacrm.ico"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\utils\config\assistant\antiqua_assistant.exe"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\Antiqua\Core\libAntiquaCRM.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\Antiqua\Image\libAntiquaImage.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\Antiqua\Printing\libAntiquaPrinting.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\Antiqua\Providers\libAntiquaProviders.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\Antiqua\Tabs\libAntiquaTabs.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\Antiqua\Widgets\libAntiquaWidgets.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\pkgbuild\qt.conf"; DestDir: "{app}"; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\i18n\antiquacrm_de.qm"; DestDir: "{app}\i18n"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\pgsql\*.sql"; DestDir: "{app}\data\pgsql\"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\json\*.json"; DestDir: "{app}\data\json\"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\fonts\*.ttf"; DestDir: "{app}\data\fonts"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\fonts\*.ttf"; DestDir: "{app}\data\fonts"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\data\documents\*.txt"; DestDir: "{app}\data\documents"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\providers\Abebooks\libAbebooks.dll"; DestDir: "{app}\plugins\antiquacrm\providers\Abebooks"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\providers\Booklooker\libBooklooker.dll"; DestDir: "{app}\plugins\antiquacrm\providers\Booklooker"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\providers\Buchfreund\libBuchfreund.dll"; DestDir: "{app}\plugins\antiquacrm\providers\Buchfreund"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\books\libBooks.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\Books"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\cdsvinyl\libCDsVinyl.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\CDsVinyl"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\reports\libReports.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\Reports"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\statistics\libStatistics.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\Statistics"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\stitches\libStitches.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\Stitches"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\various\libVarious.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\Various"; Attribs: system; Flags: ignoreversion;
Source: "{#BUILD_DIR}\src\plugins\antiquacrm\tabs\views\libViews.dll"; DestDir: "{app}\plugins\antiquacrm\tabs\Views"; Attribs: system; Flags: ignoreversion;
Source: "{#TPL_DIR}\*.png"; DestDir: "{app}\icons"; Flags: ignoreversion;
Source: "{#TPL_DIR}\*.ico"; DestDir: "{app}\icons"; Flags: ignoreversion;
;; QREncode
Source: "{#QRENCODE_PATH}\bin\libiconv-2.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QRENCODE_PATH}\bin\libpng16.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QRENCODE_PATH}\bin\libqrencode.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; PostgreSQL
Source: "{#PgSQL_PATH}\lib*.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#PgSQL_PATH}\zlib1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; MinGW NOTE: "Force overwrite at this position"
Source: "{#QTSDK_PATH}\bin\libgcc_s_seh-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\libstdc++-6.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\libwinpthread-1.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\opengl32sw.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
;; Qt6
Source: "{#QTSDK_PATH}\bin\Qt6Core.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Gui.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Widgets.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Network.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Sql.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Svg.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6SvgWidgets.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Xml.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6PrintSupport.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\bin\Qt6Charts.dll"; DestDir: "{app}"; Attribs: system; Flags: ignoreversion;
Source: "{#QTSDK_PATH}\plugins\generic\*.dll"; DestDir: "{app}\plugins\generic"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QTSDK_PATH}\plugins\imageformats\*.dll"; DestDir: "{app}\plugins\imageformats"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QTSDK_PATH}\plugins\iconengines\*.dll"; DestDir: "{app}\plugins\iconengines"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QTSDK_PATH}\plugins\platforms\*.dll"; DestDir: "{app}\plugins\platforms"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QTSDK_PATH}\plugins\sqldrivers\qsqlpsql.dll"; DestDir: "{app}\plugins\sqldrivers"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#QTSDK_PATH}\plugins\tls\*.dll"; DestDir: "{app}\plugins\tls"; Attribs: system; Flags: ignoreversion recursesubdirs createallsubdirs;
;; Kundendaten
Source: "{#COPY_DIR}\URWChancery_L-Medium-Italic.ttf"; DestDir: "{app}\company"; Flags: ignoreversion;
Source: "{#COPY_DIR}\druck_header.png"; DestDir: "{app}\company"; Flags: ignoreversion;
Source: "{#COPY_DIR}\Zertifikat\*.crt"; DestDir: "{app}\data\certs\"; Attribs: system; Flags: ignoreversion;
;; Curl CA-Bundle
Source: "{#CURL_HOME}\bin\curl-ca-bundle.crt"; DestDir: "{app}\"; Attribs: system; Flags: ignoreversion;

[Icons]
Name: "{group}\{#MY_APPL_DISPLAY_NAME}"; Filename: "{app}\{#MY_APPL_BINARY}"; IconFilename: "{app}\{#MY_APPL_NAME}.ico";
Name: "{group}\{cm:UninstallProgram,{#MY_APPL_NAME}}"; Filename: "{uninstallexe}"; IconFilename: "{app}\{#MY_APPL_NAME}.ico";
Name: "{commondesktop}\{#MY_APPL_DISPLAY_NAME}"; Filename: "{app}\{#MY_APPL_BINARY}"; Tasks: "application"; IconFilename: "{app}\{#MY_APPL_NAME}.ico";

[Registry]
;; Computer\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\HJCMS\
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_UUID}";
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_UUID}"; ValueType: string; ValueName: "Application"; ValueData: "{#MY_APPL_NAME}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_UUID}"; ValueType: string; ValueName: "Version"; ValueData: "{#MY_APPL_VERSION}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_UUID}"; ValueType: string; ValueName: "Author"; ValueData: "{#MY_APPL_PUBLISHER}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_UUID}"; ValueType: string; ValueName: "Homepage"; ValueData: "{#MY_APPL_HOMEPAGE}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: "Software\HJCMS\{#MY_APPL_UUID}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{commonpf64}\HJCMS\{#MY_APPL_BINARY}"; Flags: uninsdeletekey;

;; EOF
