# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (RESOURCE_FILES
  application.desktop.cmake
  qt.conf.cmake
  antiquacrm.png
)

SET (DOMAIN_SCOPE "de.hjcms.${PROJECTNAME}")

CONFIGURE_FILE (qt.conf.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf
)

CONFIGURE_FILE (application.desktop.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/${DOMAIN_SCOPE}.desktop
)

INSTALL (FILES
  ${DOMAIN_SCOPE}.desktop
  DESTINATION share/applications
  COMPONENT data
)

INSTALL (FILES antiquacrm.png
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps
  COMPONENT data
)

INSTALL (FILES LICENSE.md
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECTNAME}
  RENAME LICENSE
  COMPONENT data
)

