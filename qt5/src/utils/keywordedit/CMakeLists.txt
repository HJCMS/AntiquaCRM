# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaKeywords")

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
 keywordactions.cpp
 keywordedit.cpp
 keywordeditor.cpp
 keywordlabel.cpp
 keywordlabellist.cpp
 keywordlistmodel.cpp
 keywordlineedit.cpp
 keywordtable.cpp
 keywordtablemodel.cpp
)

SET (subproject_HEADERS
 keywordactions.h
 keywordedit.h
 keywordeditor.h
 keywordlabel.h
 keywordlabellist.h
 keywordlistmodel.h
 keywordlineedit.h
 keywordtable.h
 keywordtablemodel.h
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
