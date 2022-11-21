# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaProviders")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaPurchaseTable
)

SET (projectLibrary_SOURCES
 provideractiondialog.cpp
 providerbuyerinfo.cpp
 providerorderinfo.cpp
 providerpurchasebar.cpp
 providerpurchaseheader.cpp
 providerpurchasetable.cpp
 providersorderpage.cpp
 providerspageview.cpp
 providerstreeview.cpp
 tabproviders.cpp
)

SET (projectLibrary_HEADERS
 provideractiondialog.h
 providerbuyerinfo.h
 providerorderinfo.h
 providerpurchasebar.h
 providerpurchaseheader.h
 providerpurchasetable.h
 providersorderpage.h
 providerspageview.h
 providerstreeview.h
 tabproviders.h
)

ADD_LIBRARY (${_projectLibrary} STATIC
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_projectLibrary}
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Widgets
  ${projectLibrary_LINKER}
)
