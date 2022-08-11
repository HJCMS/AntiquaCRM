# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "MailForward")

SET (subproject_LINKER
 SqlCore
 Utils
)

SET (subproject_SOURCES
 mailforwarddialog.cpp
)

SET (subproject_HEADERS
 mailforwarddialog.h
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
