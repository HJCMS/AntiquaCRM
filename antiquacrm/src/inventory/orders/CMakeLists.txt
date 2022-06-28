# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "InventoryOrders")

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
 DeliverService
)

SET (subproject_SOURCES
  inventoryorders.cpp
  ordersitemlist.cpp
  orderstable.cpp
  orderstablemodel.cpp
  ordereditor.cpp
  orderpaymentstable.cpp
)

SET (subproject_HEADERS
  orderstatements.h
  ordersitemlist.h
  inventoryorders.h
  orderstable.h
  orderstablemodel.h
  ordereditor.h
  orderpaymentstable.h
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

