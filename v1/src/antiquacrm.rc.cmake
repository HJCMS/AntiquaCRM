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
// Das Betriebssystem, für das die Datei entwickelt wurde, ist unbekannt.
FILEOS          VOS_UNKNOWN
// Die Datei enthält eine Anwendung.
FILETYPE        VFT_APP
// Keine Treiber enthalten
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
      // Company that produced the file (Required).
      VALUE "CompanyName",      "HJCMS https://www.hjcms.de"
      // File description to be presented to users (Required).
      VALUE "FileDescription",  "@DESCRIPTION@"
      // Version number of the file (Required).
      VALUE "FileVersion",      "@ANTIQUACRM_VERSION_STRING@\0"
      // Internal name of the file (Required).
      VALUE "InternalName",     "@PROJECTNAME@"
      /**
        * Original name of the file, not including a path.
        * This information enables an application to determine
        * whether a file has been renamed by a user (Required).
        */
      VALUE "OriginalFilename",  VER_ORIGINALFILENAME_STR
      // Name of the product with which the file is distributed (Required).
      VALUE "ProductName",      "@DISPLAYNAME@"
      // Version of the product with which the file is distributed (Required).
      VALUE "ProductVersion",   "@ANTIQUACRM_VERSION_STRING@\0"
      //  Copyright notices that apply to the file (Optional).
      VALUE "LegalCopyright",   "Copyright(C) 2013-2022 HJCMS https://www.hjcms.de"
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
/** TODO
 * CREATEPROCESS_MANIFEST_RESOURCE_ID RT_MANIFEST QUOTE(@CMAKE_CURRENT_SOURCE_DIR@/antiqua.manifest)
 */
