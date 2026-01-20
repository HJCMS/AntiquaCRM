// vim: set fileencoding=utf-8

#include <winver.h>

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)
#define CONCAT(x,y) x ## y

#define VER_VERSIONINFO             @ANTIQUACRM_VERSION_STRING@
#define VER_PRODUCTVERSION          @ANTIQUACRM_VERSION_FILEOS@
#define VER_FILEVERSION             @ANTIQUACRM_VERSION_FILEOS@
#ifndef ANTIQUA_DEVELOPMENT
#define VER_DEBUG                   0
#else
#define VER_DEBUG                   VS_FF_DEBUG
#endif

/**
* Defines a version-information resource.
* Contains information such as the version number,
* intended operating system, and so on.
* https://docs.microsoft.com/de-de/windows/win32/menurc/versioninfo-resource?redirectedfrom=MSDN
* @{
*/
VS_VERSION_INFO VERSIONINFO
FILEVERSION     VER_FILEVERSION
PRODUCTVERSION  VER_PRODUCTVERSION

/**
 * Die Datei wurde nicht mithilfe von Standardfreigabeprozeduren erstellt.
 * Wenn dieser Wert angegeben wird, muss der StringFileInfo-Block eine
 * PrivateBuild-Zeichenfolge enthalten.
 */
FILEFLAGS       (VS_FF_PRIVATEBUILD|VER_PRERELEASE|VER_DEBUG)
FILEOS          VOS_UNKNOWN
FILETYPE        VFT_APP
FILESUBTYPE     VFT2_UNKNOWN
/**
* @}
*/

{
  BLOCK "StringFileInfo"
  {
    /* 0x0409 U.S. English AND 1200 Unicode */
    BLOCK "04091200"
    {
      VALUE "CompanyName",      "HJCMS @HOMEPAGEURL@"
      VALUE "FileDescription",  "@PROJECTNAME@"
      VALUE "FileVersion",      VER_FILEVERSION
      VALUE "InternalName",     "@PROJECTNAME@"
      VALUE "LegalCopyright",   "Copyright(C) 2013-2026 HJCMS https://www.hjcms.de"
      VALUE "LegalTrademarks",  "HJCMS @HOMEPAGEURL@"
      VALUE "OriginalFilename", "@PROJECTNAME@"
      VALUE "ProductName",      "AntiquaCRM"
      VALUE "ProductVersion",   VER_PRODUCTVERSION
    }
  }

  BLOCK "VarFileInfo"
  {
    // English language (0x409) to Unicode (1200).
    VALUE "Translation", 0x409, 1200
  }
}

#include "winuser.h"
/**
 * Defines a bitmap that defines the shape of the icon to be used for
 * a given application or an animated icon.
 */
IDI_ICON1   ICON    DISCARDABLE   QUOTE(@CMAKE_CURRENT_SOURCE_DIR@/antiquacrm.ico)

/**
 * TODO
 * CREATEPROCESS_MANIFEST_RESOURCE_ID RT_MANIFEST QUOTE(@CMAKE_CURRENT_SOURCE_DIR@/antiqua.manifest)
 */
