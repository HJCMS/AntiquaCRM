# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaTemplates")

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
)

SET (subproject_FILES
 AntiquaTemplates.cmake
)

CONFIGURE_FILE (AntiquaTemplates.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaTemplates
)

SET (subproject_SOURCES
 messagetemplates.cpp
 messageeditor.cpp
 messagekeywordlist.cpp
 messagecaller.cpp
 messageselecter.cpp
 antiquatemplates.cpp
)

SET (subproject_HEADERS
 messagetemplates.h
 messageeditor.h
 messagekeywordlist.h
 messagecaller.h
 messageselecter.h
 antiquatemplates.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  ${subproject_FILES}
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

