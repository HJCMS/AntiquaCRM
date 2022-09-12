# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "MessageTemplates")

SET (subproject_LINKER
 SqlCore
 Utils
)

SET (subproject_SOURCES
 messagetemplates.cpp
 messageeditor.cpp
 messagekeywordlist.cpp
 messageselecter.cpp
)

SET (subproject_HEADERS
 messagetemplates.h
 messageeditor.h
 messagekeywordlist.h
 messageselecter.h
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

