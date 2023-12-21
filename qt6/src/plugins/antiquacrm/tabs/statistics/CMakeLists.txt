# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_pluginTarget "Statistics")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

SET (plugin_FILES
  statistics.json
)

CONFIGURE_FILE (antiquastatistics.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquastatistics.h
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_pluginTarget} MODULE
  ${plugin_FILES} AntiquaStatistics
  antiquastatistics.h.cmake antiquastatistics.h
  achartview.h achartview.cpp
  statsutils.h statsutils.cpp
  statisticsmenu.h statisticsmenu.cpp
  tabstatistics.h tabstatistics.cpp
  statisticsindex.h statisticsindex.cpp
  statisticsconfigwidget.h statisticsconfigwidget.cpp
  statisticsselecter.h statisticsselecter.cpp
  salesinmonth.h salesinmonth.cpp
  statsbarseries.h statsbarseries.cpp
  salesfromproviders.h salesfromproviders.cpp
  salescategories.h salescategories.cpp
  countaxis.h countaxis.cpp
  yearaxis.h yearaxis.cpp
  dailyaverage.h dailyaverage.cpp
  soldcountchart.h soldcountchart.cpp
  salesvolumechart.h salesvolumechart.cpp
  monthbarset.h monthbarset.cpp
  statisticsviewarea.h statisticsviewarea.cpp
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
)

TARGET_LINK_LIBRARIES (${_pluginTarget} PRIVATE
 Qt6::Core
 Qt6::Widgets
 Qt6::Sql
 Qt6::Network
 Qt6::Xml
 Qt6::Charts
 AntiquaCRM
 AntiquaTabs
 AntiquaWidgets
)

##EOF
