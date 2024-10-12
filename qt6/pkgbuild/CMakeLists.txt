# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (TARGET_FILES
 qt.conf.cmake
 antiquacrm.iss.cmake
 antiquacrm.spec.cmake
 antiquacrm.png
 antiquacrm.ico
 LICENSE.md
)

###
## \Section Configure Windows files
## BEGIN WIN_OS
###
IF (CMAKE_HOST_WIN32)
 CONFIGURE_FILE (antiquacrm.iss.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/antiquacrm.iss
  NEWLINE_STYLE CRLF
 )
 FILE (COPY antiquacrm.ico
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
 )
 CONFIGURE_FILE (qt.conf.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
 )
 FILE (COPY LICENSE.md
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
 )
ENDIF (CMAKE_HOST_WIN32)
###
## END WIN_OS
###

###
## \Section Configure Linux files
## BEGIN LINUX_OS
###
IF (CMAKE_HOST_UNIX)
  INSTALL (FILES antiquacrm.png
    DESTINATION share/pixmaps
    COMPONENT data
  )

  ## RPM
  STRING (TIMESTAMP ANTIQUACRM_PACKAGE_RELEASE "%j")
  CONFIGURE_FILE (antiquacrm.spec.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.spec
  )

ENDIF (CMAKE_HOST_UNIX)
###
## END LINUX_OS
###
