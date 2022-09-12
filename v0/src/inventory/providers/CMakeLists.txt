# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "InventoryProviders")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 SqlCore
 AntiquaCore
 Settings
 Networking
 Utils
 Imaging
 Printing
)

SET (subproject_SOURCES
  inventoryproviders.cpp
  providerselectcustomer.cpp
  providerstoolbar.cpp
  providerstreeview.cpp
  providerspageview.cpp
)

SET (subproject_HEADERS
  providersstatements.h
  providerselectcustomer.h
  providerstoolbar.h
  providerstreeview.h
  providerspageview.h
  inventoryproviders.h
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
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  ${subproject_LINKER}
)
