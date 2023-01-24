# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (RESOURCE_FILES
 application.desktop.cmake
 assistant.desktop.cmake
 qt.conf.cmake
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
 )
 FILE (COPY antiquacrm.ico
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
 )
 CONFIGURE_FILE (qt.conf.cmake
   ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf
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
STRING (TIMESTAMP ANTIQUACRM_PACKAGE_RELEASE "%j")
IF (CMAKE_HOST_UNIX)
CONFIGURE_FILE (application.desktop.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${ANTIQUACRM_DOMAIN_SCOPE}.desktop
)

CONFIGURE_FILE (assistant.desktop.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${ANTIQUACRM_DOMAIN_SCOPE}_assistant.desktop
)

INSTALL (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${ANTIQUACRM_DOMAIN_SCOPE}.desktop
  ${CMAKE_CURRENT_BINARY_DIR}/${ANTIQUACRM_DOMAIN_SCOPE}_assistant.desktop
  DESTINATION share/applications
  COMPONENT data
)

INSTALL (FILES antiquacrm.png
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps
  COMPONENT data
)

## RPM
CONFIGURE_FILE (antiquacrm.spec.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/antiquacrm.spec
)

ENDIF (CMAKE_HOST_UNIX)
###
## END LINUX_OS
###
