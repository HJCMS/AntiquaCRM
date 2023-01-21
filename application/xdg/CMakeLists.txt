# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (RESOURCE_FILES
 application.desktop.cmake
 assistant.desktop.cmake
 qt.conf.cmake
 antiquacrm.png
 antiquacrm.ico
)

SET (DOMAIN_SCOPE "de.hjcms.${PROJECTNAME}")

CONFIGURE_FILE (qt.conf.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf
)

CONFIGURE_FILE (application.desktop.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/${DOMAIN_SCOPE}.desktop
)

CONFIGURE_FILE (assistant.desktop.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/${DOMAIN_SCOPE}_assistant.desktop
)

## Package building release
STRING (TIMESTAMP ANTIQUACRM_PACKAGE_RELEASE "%j")
CONFIGURE_FILE (antiquacrm.iss.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.iss
)
CONFIGURE_FILE (antiquacrm.spec.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.spec
)

## Desktop specification
IF (CMAKE_HOST_WIN32)
 FILE (COPY antiquacrm.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSE (CMAKE_HOST_WIN32)
INSTALL (FILES ${DOMAIN_SCOPE}.desktop ${DOMAIN_SCOPE}_assistant.desktop
  DESTINATION share/applications
  COMPONENT data
)
## pixmap
INSTALL (FILES antiquacrm.png
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps
  COMPONENT data
)
## COPRIGHT
INSTALL (FILES LICENSE.md
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECTNAME}
  RENAME LICENSE
  COMPONENT data
)
ENDIF (CMAKE_HOST_WIN32)
