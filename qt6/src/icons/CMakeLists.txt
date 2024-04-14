# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

IF (CMAKE_HOST_UNIX)
INSTALL (FILES antiquacrm.png
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps
  COMPONENT data
)
ENDIF (CMAKE_HOST_UNIX)
