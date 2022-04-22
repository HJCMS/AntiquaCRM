# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "InventoryCustomers")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
 customeradditional.cpp
 customerbillinginfo.cpp
 customercontact.cpp
 customeroverview.cpp
 editcustomer.cpp
 genderbox.cpp
 salutationbox.cpp
 inventorycustomers.cpp
 phoneedit.cpp
 emailedit.cpp
)

SET (subproject_HEADERS
 customeradditional.h
 customerbillinginfo.h
 customercontact.h
 customeroverview.h
 editcustomer.h
 genderbox.h
 salutationbox.h
 inventorycustomers.h
 phoneedit.h
 emailedit.h
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
)

