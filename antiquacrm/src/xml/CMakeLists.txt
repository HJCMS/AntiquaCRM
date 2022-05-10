# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (xml_FILES
  ib_publisher_de.xml
)

file(COPY ${xml_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

INSTALL (FILES ${xml_FILES}
  DESTINATION share/${PROJECTNAME}/xml
  COMPONENT data
)
