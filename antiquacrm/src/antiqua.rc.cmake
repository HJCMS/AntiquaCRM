
#include <winver.h>

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)
#define CONCAT(x,y) x ## y

FILEVERSION     1,0,0,0
PRODUCTVERSION  1,0,0,0
BEGIN
  BLOCK "StringFileInfo"
  BEGIN
    BLOCK "080904E4"
    BEGIN
      VALUE "CompanyName", "HJCMS https://www.hjcms.de"
      VALUE "FileDescription", "@DESCRIPTION@"
      VALUE "FileVersion", "1.0"
      VALUE "InternalName", "@PROJECTNAME@"
      VALUE "LegalCopyright", "Copyright(C) 2013-2022 HJCMS https://www.hjcms.de"
      VALUE "OriginalFilename", "@PROJECTNAME@.exe"
      VALUE "ProductName", "@DISPLAYNAME@"
      VALUE "ProductVersion", "@ANTIQUACRM_VERSION_STRING@"
    END
  END
  BLOCK "VarFileInfo"
  BEGIN
    VALUE "Translation", 0x809, 1252
  END
END

#include "winuser.h"

IDI_ICON1   ICON    DISCARDABLE   QUOTE(@CMAKE_CURRENT_SOURCE_DI@/antiqua.ico)

