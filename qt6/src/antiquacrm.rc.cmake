// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include <winver.h>

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)
#define CONCAT(x,y) x ## y

#define VER_PRODUCTVERSION          @ANTIQUACRM_VERSION_FILEOS@
#define VER_FILEVERSION             @ANTIQUACRM_VERSION_FILEOS@
#define VER_ORIGINALFILENAME_STR    QUOTE(@PROJECTNAME@)

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
FILEFLAGS       (VS_FF_PRIVATEBUILD)
FILEOS          VOS_UNKNOWN
FILETYPE        VFT_APP
FILESUBTYPE     VFT2_UNKNOWN
/**
* @}
*/

{
  BLOCK "StringFileInfo"
  {
    /* 0x0409 U.S. English AND 04B0 Unicode */
    BLOCK "040904B0"
    {
      VALUE "CompanyName",      "HJCMS @HOMEPAGEURL@"
      VALUE "FileDescription",  "@DESCRIPTION@"
      VALUE "FileVersion",      "@ANTIQUACRM_VERSION_STRING@\0"
      VALUE "InternalName",     "@PROJECTNAME@"
      VALUE "OriginalFilename",  VER_ORIGINALFILENAME_STR
      VALUE "AppId",            "@ANTIQUACRM_APPID@"
      VALUE "ProductName",      "@DISPLAYNAME@"
      VALUE "ProductVersion",   "@ANTIQUACRM_VERSION_STRING@\0"
      VALUE "LegalCopyright",   "Copyright(C) 2013-2024 HJCMS https://www.hjcms.de"
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
