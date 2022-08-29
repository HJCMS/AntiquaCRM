# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "HomeBase3")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CURL_INCLUDE_DIRS}
)

SET (subproject_FILES
 # @moved to ../../xml/BookInventory.xsd
)

SET (subproject_SOURCES
  booklisting.cpp
  booklistingnode.cpp
  booknodetype.cpp
  homebase3.cpp
  homebaseschema.cpp
  homebasebuilder.cpp
  homebasereader.cpp
  ${subproject_FILES}
)

SET (subproject_HEADERS
  booklisting.h
  booklistingnode.h
  booknodetype.h
  homebase3.h
  homebaseschema.h
  homebasebuilder.h
  homebaseconfig.h
  homebasereader.h
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_subproject}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Xml
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS ${_subproject}
    LIBRARY DESTINATION lib${LIB_SUFFIX}/
    COMPONENT Libraries
    NAMELINK_COMPONENT Development
  )

  INSTALL (FILES ${subproject_HEADERS}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT Development
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS ${_subproject}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/
    COMPONENT Libraries
  )
ENDIF (ANTIQUA_LSB_INSTALL)
