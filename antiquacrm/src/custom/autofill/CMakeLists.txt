# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Autofill")

SET (subproject_LINKER
 AntiquaCore
)

SET (subproject_SOURCES
  autofill.cpp
)

SET (subproject_HEADERS
  autofill.h
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
  Qt${QT_VERSION_MAJOR}::Xml
  ${subproject_LINKER}
)
