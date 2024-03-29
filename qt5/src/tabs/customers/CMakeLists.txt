# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaCustomers")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaMailing
 AntiquaPrinting
)

SET (projectLibrary_SOURCES
 tabcustomers.cpp
 customerstablemodel.cpp
 customerstableview.cpp
 customersstatusbar.cpp
 customersselectfilter.cpp
 customerssearchlineedit.cpp
 customerssearchbar.cpp
 customerseditor.cpp
 customersheaderframe.cpp
 customerstabwidget.cpp
 customersdata.cpp
 customersbillings.cpp
 customersorders.cpp
)

SET (projectLibrary_HEADERS
 tabcustomers.h
 customerstablemodel.h
 customerstableview.h
 customersstatusbar.h
 customersselectfilter.h
 customerssearchlineedit.h
 customerssearchbar.h
 customerseditor.h
 customersheaderframe.h
 customerstabwidget.h
 customersdata.h
 customersbillings.h
 customersorders.h
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
