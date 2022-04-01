# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "InventoryBooks")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
  inventorybooks.cpp
  searchbookbar.cpp
  bookstableview.cpp
  bookstablemodel.cpp
  combofieldbox.cpp
  searchlineedit.cpp
)

SET (subproject_HEADERS
  inventorybooks.h
  searchbookbar.h
  bookstableview.h
  bookstablemodel.h
  combofieldbox.h
  searchlineedit.h
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
)

