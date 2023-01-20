# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaViews")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
)

SET (subproject_SOURCES
 tabviews.cpp
 viewsactiongroup.cpp
 viewstablemodel.cpp
)

SET (subproject_HEADERS
 tabviews.h
 viewsactiongroup.h
 viewstablemodel.h
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
 Qt${QT_VERSION_MAJOR}::Core
 Qt${QT_VERSION_MAJOR}::Widgets
 Qt${QT_VERSION_MAJOR}::Sql
 ${subproject_LINKER}
)
