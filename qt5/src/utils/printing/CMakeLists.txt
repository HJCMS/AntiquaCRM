# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaPrinting")

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaCache
 ${QRENCODE_LIBRARY}
)

INCLUDE_DIRECTORIES (
 ${QRENCODE_INCLUDE_DIRS}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${PROJECT_INCLUDE_DIRS}
)

SET (subproject_FILES
 AntiquaPrinting.cmake
)

CONFIGURE_FILE (AntiquaPrinting.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaPrinting
)

SET (subproject_SOURCES
  printing.cpp
  deliverynote.cpp
  paymentreminder.cpp
  invoice.cpp
  bookcard.cpp
  reports.cpp
  texteditor.cpp
)

SET (subproject_HEADERS
  printing.h
  deliverynote.h
  paymentreminder.h
  invoice.h
  bookcard.h
  reports.h
  texteditor.h
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
  Qt${QT_VERSION_MAJOR}::PrintSupport
  ${subproject_LINKER}
)
