# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (text_FILES
 welcome_de.txt
 database_de.txt
)

IF (CMAKE_HOST_WIN32)
 FILE (COPY ${text_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
 ## only used in test environment
 FILE (COPY ${text_FILES} DESTINATION
   ${CMAKE_CURRENT_BINARY_DIR}/../../utils/config/assistant/data/documents)
ELSE (CMAKE_HOST_WIN32)
 INSTALL (FILES ${font_FILES}
   DESTINATION share/${PROJECTNAME}/data/documents
   COMPONENT data
 )
ENDIF (CMAKE_HOST_WIN32)
