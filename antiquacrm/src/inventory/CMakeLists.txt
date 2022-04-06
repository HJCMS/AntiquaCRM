# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Inventory")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
  articleid.cpp
  editionedit.cpp
  editordialog.cpp
  inventory.cpp
  isbnedit.cpp
  priceedit.cpp
  searchbar.cpp
  setlanguage.cpp
  storageedit.cpp
  strlineedit.cpp
  yearedit.cpp
)

SET (subproject_HEADERS
  articleid.h
  editionedit.h
  editordialog.h
  inventory.h
  isbnedit.h
  priceedit.h
  setlanguage.h
  searchbar.h
  storageedit.h
  strlineedit.h
  yearedit.h
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
  Qt${QT_VERSION_MAJOR}::Sql
)

