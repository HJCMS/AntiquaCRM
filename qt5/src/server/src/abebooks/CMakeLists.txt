# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AbeBooks")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (${PROJECT_INCLUDE_DIRS})

SET (subproject_SOURCES
 abebooks.cpp
 abebooksdocument.cpp
)

SET (subproject_HEADERS
 abebooks.h
 abebooksdocument.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  ${PROJECT_LIBRARIES}
)
