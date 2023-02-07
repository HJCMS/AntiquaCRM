# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (json_data_FILES
 european_countries.json
 iso_countrycodes.json
 id3taglist.json
)

IF (CMAKE_HOST_WIN32)
 FILE (COPY ${json_data_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSE (CMAKE_HOST_WIN32)
 INSTALL (FILES ${json_data_FILES}
   DESTINATION share/${PROJECTNAME}/data/json
   COMPONENT data
 )
ENDIF (CMAKE_HOST_WIN32)
