# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaStatistics")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${Qt${QT_VERSION_MAJOR}Charts_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
)

SET (subproject_SOURCES
 ahorizontalbarseries.cpp
 categoriesinyear.cpp
 countaxis.cpp
 paymentsinyear.cpp
 statisticsactiongroup.cpp
 statsutils.cpp
 tabstatistics.cpp
 yearaxis.cpp
)

SET (subproject_HEADERS
 ahorizontalbarseries.h
 categoriesinyear.h
 countaxis.h
 paymentsinyear.h
 statisticsactiongroup.h
 statsutils.h
 tabstatistics.h
 yearaxis.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

TARGET_COMPILE_FEATURES(${_subproject} PUBLIC cxx_std_17)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
 Qt${QT_VERSION_MAJOR}::Core
 Qt${QT_VERSION_MAJOR}::Widgets
 Qt${QT_VERSION_MAJOR}::Sql
 Qt${QT_VERSION_MAJOR}::Charts
 ${subproject_LINKER}
)
