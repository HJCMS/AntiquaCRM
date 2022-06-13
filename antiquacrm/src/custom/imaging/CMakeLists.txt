# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Imaging")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 SqlCore
 AntiquaCore
)

SET (subproject_SOURCES
  imagedialog.cpp
  imageview.cpp
  sourceinfo.cpp
)

SET (subproject_HEADERS
  imagedialog.h
  imageview.h
  sourceinfo.h
  Imaging
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
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  ${subproject_LINKER}
)
