# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaOrders")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaPurchaseTable
 AntiquaMailing
 AntiquaPrinting
 AntiquaReturnOrder
)

SET (projectLibrary_SOURCES
 taborders.cpp
 orderaddarticle.cpp
 orderssearchbar.cpp
 ordershistorybutton.cpp
 ordersstatusbar.cpp
 orderstablemodel.cpp
 orderstableview.cpp
 orderseditor.cpp
 orderscustomerinfo.cpp
 orderscostsettings.cpp
 ordersitemlist.cpp
 ordersreturning.cpp
 orderscustomsearch.cpp
)

SET (projectLibrary_HEADERS
 taborders.h
 orderaddarticle.h
 orderssearchbar.h
 ordershistorybutton.h
 ordersstatusbar.h
 orderstablemodel.h
 orderstableview.h
 orderseditor.h
 orderscustomerinfo.h
 orderscostsettings.h
 ordersitemlist.h
 ordersreturning.h
 orderscustomsearch.h
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
