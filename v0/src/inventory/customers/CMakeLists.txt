# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "InventoryCustomers")

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
 MailForward
)

SET (subproject_SOURCES
  customerbillinginfo.cpp
  customerpayments.cpp
  customercontact.cpp
  customertableview.cpp
  customerstablemodel.cpp
  editcustomer.cpp
  inventorycustomers.cpp
)

SET (subproject_HEADERS
  customerstatements.h
  customerbillinginfo.h
  customerpayments.h
  customercontact.h
  customertableview.h
  customerstablemodel.h
  editcustomer.h
  inventorycustomers.h
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
