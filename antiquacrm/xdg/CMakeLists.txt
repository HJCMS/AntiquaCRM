# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (RESOURCE_FILES
  desktop.cmake
)

SET (DOMAIN_SCOPE "de.hjcms.${PROJECTNAME}")

CONFIGURE_FILE (desktop.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/${DOMAIN_SCOPE}.desktop
)

INSTALL (FILES
  ${DOMAIN_SCOPE}.desktop
  DESTINATION share/applications
  COMPONENT data
)

CONFIGURE_FILE (qt.conf.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf
)
