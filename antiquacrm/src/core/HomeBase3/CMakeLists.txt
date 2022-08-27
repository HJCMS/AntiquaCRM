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
 BookInventory.xsd
)

SET (subproject_SOURCES
 homebase3.cpp
 homebasebuilder.cpp
 homebasereader.cpp
 booklisting.cpp
 booklistingnode.cpp
 titlenode.cpp
)

SET (subproject_HEADERS
 homebaseconfig.h
 homebase3.h
 homebasebuilder.h
 homebasereader.h
 booklisting.h
 booklistingnode.h
 titlenode.h
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  ${subproject_FILES}
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
