# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (xml_FILES
  ib_publisher_de.xml
  eu_countries.xml
  categories_abebooks.xml
  categories_booklooker.xml
)

SET (source_FILES
  ${xml_FILES}
)

file(COPY ${xml_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (FILES ${source_FILES}
    DESTINATION share/${PROJECTNAME}/xml
    COMPONENT data
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (FILES ${source_FILES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/xml
    COMPONENT data
  )
ENDIF (ANTIQUA_LSB_INSTALL)
