# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaVarious")

SET (projectLibrary_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaKeywords
 AntiquaImgEdit
 AntiquaPrinting
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_SOURCES
 varioussearchedit.cpp
 varioussearchbar.cpp
 varioustablemodel.cpp
 varioustableview.cpp
 variousstatusbar.cpp
 variouseditor.cpp
 tabvarious.cpp
)

SET (projectLibrary_HEADERS
 varioussearchedit.h
 varioussearchbar.h
 varioustablemodel.h
 varioustableview.h
 variousstatusbar.h
 variouseditor.h
 tabvarious.h
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
