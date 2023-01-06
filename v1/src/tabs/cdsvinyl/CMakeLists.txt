# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaCDsVinyl")

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
 tabcdvinyl.cpp
 cdvfilterselect.cpp
 cdvsearchline.cpp
 cdvsearchbar.cpp
 cdvtablemodel.cpp
 cdvtableview.cpp
 cdvstatusbar.cpp
 cdveditor.cpp
)

SET (projectLibrary_HEADERS
 tabcdvinyl.h
 cdvfilterselect.h
 cdvsearchline.h
 cdvsearchbar.h
 cdvtablemodel.h
 cdvtableview.h
 cdvstatusbar.h
 cdveditor.h
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
