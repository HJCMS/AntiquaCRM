# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "InventoryBooks")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 SqlCore
 AntiquaCore
 Settings
 Networking
 Utils
 Printing
 Imaging
 CategoryEdit
 KeywordEdit
)

SET (subproject_SOURCES
 bookeditor.cpp
 bookstable.cpp
 bookstablemodel.cpp
 isbnresults.cpp
 booksearchbar.cpp
 inventorybooks.cpp
)

SET (subproject_HEADERS
 bookstatements.h
 bookeditor.h
 bookstable.h
 bookstablemodel.h
 isbnresults.h
 booksearchbar.h
 inventorybooks.h
)

## TODO ADD_DEFINITION(BOOK_INVENTORY_DEBUG);

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

