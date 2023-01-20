# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (font_FILES
 barcodefont.ttf
 steve.ttf
)

IF (CMAKE_HOST_WIN32)
 FILE (COPY ${font_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSE (CMAKE_HOST_WIN32)
 INSTALL (FILES ${font_FILES}
   DESTINATION share/${PROJECTNAME}/data/fonts
   COMPONENT data
 )
ENDIF (CMAKE_HOST_WIN32)
