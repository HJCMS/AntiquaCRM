# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_slibproject "HomeBase3")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
  booklisting.cpp
  booklistingnode.cpp
  booknodetype.cpp
  homebasebuilder.cpp
  homebasereader.cpp
  ${subproject_FILES}
)

SET (subproject_HEADERS
  booklisting.h
  booklistingnode.h
  booknodetype.h
  homebasebuilder.h
  homebaseconfig.h
  homebasereader.h
)

ADD_LIBRARY (${_slibproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

SET_TARGET_PROPERTIES (${_slibproject} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_slibproject}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_slibproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Xml
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS ${_slibproject}
    LIBRARY DESTINATION lib${LIB_SUFFIX}/
    COMPONENT Libraries
    NAMELINK_COMPONENT Development
  )

  INSTALL (FILES ${subproject_HEADERS}
    DESTINATION include/${_slibproject}
    COMPONENT Development
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS ${_slibproject}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/
    COMPONENT Libraries
  )
ENDIF (ANTIQUA_LSB_INSTALL)
