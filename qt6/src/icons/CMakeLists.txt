# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

ADD_SUBDIRECTORY (oxygen)

SET (icons_FILES
 antiquacrm.png
)

IF (CMAKE_HOST_UNIX)
INSTALL (FILES antiquacrm.png
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps
  COMPONENT data
)
ENDIF (CMAKE_HOST_UNIX)
