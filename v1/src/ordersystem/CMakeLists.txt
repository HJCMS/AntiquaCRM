# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaOrderSystem")

CONFIGURE_FILE (OrderSystem.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/OrderSystem
)

SET (subproject_LINKER
 AntiquaCRM
)

SET (subproject_SOURCES
 ordersystem.cpp
 ordersystemdatabase.cpp
)

SET (subproject_HEADERS
 ordersystem.h
 ordersystemdatabase.h
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
  Qt${QT_VERSION_MAJOR}::Sql
  ${subproject_LINKER}
)

