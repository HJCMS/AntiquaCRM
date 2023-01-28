# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaDesignations")

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
 designationsdialog.cpp
 designationslist.cpp
 designationedit.cpp
)

SET (subproject_HEADERS
 designationsdialog.h
 designationdata.h
 designationslist.h
 designationedit.h
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
