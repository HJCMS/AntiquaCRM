# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (abebooks_SOURCES
 abebooks.json
 config.xml
 abebooks.cpp
 abebooksdocument.cpp
 abebooksifacewidget.cpp
 abebooksremoteactions.cpp
 abebooksrequester.cpp
)

SET (abebooks_HEADERS
 abebooksconfig.h
 abebooks.h
 abebooksdocument.h
 abebooksifacewidget.h
 abebooksremoteactions.h
 abebooksrequester.h
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

ADD_LIBRARY (abebooks MODULE
  ${abebooks_SOURCES}
  ${abebooks_HEADERS}
)

SET_TARGET_PROPERTIES (abebooks PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME abebooks
)

TARGET_LINK_LIBRARIES (abebooks PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Xml
  Qt${QT_VERSION_MAJOR}::Network
  AntiquaCore
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS abebooks
    LIBRARY DESTINATION lib${LIB_SUFFIX}/antiqua/plugins
    COMPONENT Plugin
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS abebooks
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins/antiqua/
    COMPONENT Plugin
  )
ENDIF (ANTIQUA_LSB_INSTALL)
