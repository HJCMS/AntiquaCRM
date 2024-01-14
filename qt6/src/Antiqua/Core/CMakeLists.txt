# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_sharedLibrary "AntiquaCRM")

INCLUDE_DIRECTORIES (
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

CONFIGURE_FILE (aglobal.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/aglobal.h
)

CONFIGURE_FILE (AntiquaCRM.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRM
)

SET (projectLibrary_FILES
 Core.dox
 aglobal.h.cmake
 AntiquaCRM.cmake
)

SET (projectLibrary_SOURCES
 anetworkcache.cpp
 anetworker.cpp
 anetworkiface.cpp
 anetworkrequest.cpp
 aproviderorders.cpp
 areceiver.cpp
 asettings.cpp
 aeucountries.cpp
 ashareddatafiles.cpp
 asqlcore.cpp
 asqldataquery.cpp
 asqlfiles.cpp
 asqlprofile.cpp
 asqlquerymodel.cpp
 asqlsettings.cpp
 atableheadercolumn.cpp
 ataxcalculator.cpp
 atransmitter.cpp
 autil.cpp
 aorderimport.cpp
)

SET (projectLibrary_HEADERS
 aglobal.h
 aenumerations.h
 aflags.h
 anetworkcache.h
 anetworker.h
 anetworkiface.h
 anetworkrequest.h
 aproviderorders.h
 areceiver.h
 asettings.h
 aeucountries.h
 ashareddatafiles.h
 asqlcore.h
 asqldataquery.h
 asqlfiles.h
 asqlprofile.h
 asqlquerymodel.h
 asqlsettings.h
 atableheadercolumn.h
 ataxcalculator.h
 atransmitter.h
 autil.h
 aorderimport.h
 AntiquaCRM
 AGlobal
 ASettings
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_sharedLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_sharedLibrary}
  LINKER_LANGUAGE CXX
)

IF(Qt6DBus_FOUND)
  ADD_DEFINITIONS (-DANTIQUACRM_DBUS_ENABLED)
  TARGET_LINK_LIBRARIES (${_sharedLibrary} PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::Xml
    Qt6::DBus
  )
ELSE()
  TARGET_LINK_LIBRARIES (${_sharedLibrary} PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::Xml
  )
ENDIF()

INSTALL (TARGETS ${_sharedLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)

#EOF
