# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaCRM")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ZLIB_INCLUDE_DIRS}
)

CONFIGURE_FILE (aglobal.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/aglobal.h
)

CONFIGURE_FILE (AntiquaCRM.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRM
)

SET (projectLibrary_FILES
 aglobal.h.cmake
 AntiquaCRM.cmake
)

SET (projectinc_FILES
 AntiquaCRM
 AGlobal
 ASettings
)

SET (projectLibrary_SOURCES
 acompleterdata.cpp
 aeuropeancountries.cpp
 anetworker.cpp
 anetworkiface.cpp
 anetworkrequest.cpp
 aproviderorders.cpp
 areceiver.cpp
 asettings.cpp
 ashareddatafiles.cpp
 asqlcore.cpp
 asqldataquery.cpp
 asqlfiles.cpp
 asqlprofile.cpp
 asqlquerymodel.cpp
 asqlsettings.cpp
 ataxcalculator.cpp
 atransmitter.cpp
 autil.cpp
)

SET (projectLibrary_HEADERS
 acompleterdata.h
 aeuropeancountries.h
 aglobal.h
 anetworker.h
 anetworkiface.h
 anetworkrequest.h
 aproviderorders.h
 areceiver.h
 asettings.h
 ashareddatafiles.h
 asqlcore.h
 asqldataquery.h
 asqlfiles.h
 asqlprofile.h
 asqlquerymodel.h
 asqlsettings.h
 ataxcalculator.h
 atransmitter.h
 autil.h
 ${projectinc_FILES}
)

ADD_LIBRARY (${_projectLibrary} SHARED
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_projectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${ZLIB_LIBRARIES}
)

INSTALL (TARGETS ${_projectLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)
