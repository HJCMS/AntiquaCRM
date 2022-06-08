# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Printing")

SET (subproject_LINKER
 AntiquaCore
 Settings
)

SET (subproject_SOURCES
  printing.cpp
  deliverynote.cpp
  invoice.cpp
  bookcard.cpp
  texteditor.cpp
)

SET (subproject_HEADERS
  printing.h
  deliverynote.h
  invoice.h
  bookcard.h
  texteditor.h
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
  Qt${QT_VERSION_MAJOR}::PrintSupport
  Qt${QT_VERSION_MAJOR}::Xml
  ${subproject_LINKER}
)
