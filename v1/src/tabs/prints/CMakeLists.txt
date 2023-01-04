# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaPrintsStitches")

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
 AntiquaPrinting
)

SET (projectLibrary_SOURCES
 tabprintsstitches.cpp
 printsstitcheseditor.cpp
 printsstitchesfilterselect.cpp
 printsstitchessearchline.cpp
 printsstitchessearchbar.cpp
 printsstitchesmodel.cpp
 printsstitchestableview.cpp
 printsstitchesstatusbar.cpp
)

SET (projectLibrary_HEADERS
 tabprintsstitches.h
 printsstitcheseditor.h
 printsstitchesfilterselect.h
 printsstitchessearchline.h
 printsstitchessearchbar.h
 printsstitchesmodel.h
 printsstitchestableview.h
 printsstitchesstatusbar.h
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
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Widgets
  ${projectLibrary_LINKER}
)
