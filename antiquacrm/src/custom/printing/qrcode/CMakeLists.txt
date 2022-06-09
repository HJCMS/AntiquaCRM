# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
  qtqrcode.cpp
  qtqrcodepainter.cpp
)

SET (subproject_HEADERS
  qtqrcode.h
  qtqrcodepainter.h
)

ADD_LIBRARY (QTQRCode STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (QTQRCode PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME QTQRCode
)

TARGET_LINK_LIBRARIES (QTQRCode PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  ${QRENCODE_LIBRARY}
)
