# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (RESOURCE_FILES
  antiquacrm_utf8.iss.cmake
  application.desktop.cmake
  qt.conf.cmake
  settings.cpp.tpl
  settings.h.tpl
)

SET (DOMAIN_SCOPE "de.hjcms.${PROJECTNAME}")

CONFIGURE_FILE (qt.conf.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf
)

CONFIGURE_FILE (antiquacrm_utf8.iss.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECTNAME}.iss
)

CONFIGURE_FILE (application.desktop.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/${DOMAIN_SCOPE}.desktop
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (FILES
    ${DOMAIN_SCOPE}.desktop
    DESTINATION share/applications
    COMPONENT data
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf
    DESTINATION ${CMAKE_INSTALL_PREFIX}/
    COMPONENT data
  )
ENDIF (ANTIQUA_LSB_INSTALL)
