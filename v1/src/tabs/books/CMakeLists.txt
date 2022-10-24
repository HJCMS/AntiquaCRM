# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaBooks")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaKeywords
 AntiquaImgEdit
)

SET (projectLibrary_SOURCES
 tabbooks.cpp
 booktableview.cpp
 bookstablemodel.cpp
 booksearchbar.cpp
 bookstatusbar.cpp
 bookselectfilter.cpp
 booksearchline.cpp
 bookeditor.cpp
)

SET (projectLibrary_HEADERS
 tabbooks.h
 booktableview.h
 bookstablemodel.h
 booksearchbar.h
 bookstatusbar.h
 bookselectfilter.h
 booksearchline.h
 bookeditor.h
)

ADD_LIBRARY (${_projectLibrary} STATIC
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_projectLibrary}
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  ${projectLibrary_LINKER}
)
