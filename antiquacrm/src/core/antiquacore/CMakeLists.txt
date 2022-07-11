# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaCore")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CURL_INCLUDE_DIRS}
)

CONFIGURE_FILE (antiqua_global.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiqua_global.h
)

CONFIGURE_FILE (interface.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/interface.h
)

SET (subproject_FILES
  README
)

SET (subproject_SOURCES
  applsettings.cpp
  trustlist.cpp
  datafield.cpp
  orderstatus.cpp
  orderarticle.cpp
  providerorders.cpp
  paymenttypeset.cpp
  purchasepaymentinfo.cpp
  purchasedebugtable.cpp
  purchaserordertable.cpp
  pluginloader.cpp
  networker.cpp
  curljson.cpp
  interface.cpp
  taxcalculator.cpp
)

SET (subproject_HEADERS
  applsettings.h
  trustlist.h
  datafield.h
  orderstatus.h
  orderarticle.h
  providerorders.h
  paymenttypeset.h
  purchasepaymentinfo.h
  purchasedebugtable.h
  purchaserordertable.h
  networker.h
  curljson.h
  AntiquaCRM
  antiqua_global.h
  pluginloader.h
  interface.h
  taxcalculator.h
  AntiquaInterface
  Networking
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_subproject}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  ${CURL_LIBRARIES}
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS ${_subproject}
    LIBRARY DESTINATION lib${LIB_SUFFIX}/
    COMPONENT Libraries
    NAMELINK_COMPONENT Development
  )

  INSTALL (FILES ${subproject_HEADERS}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT Development
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS ${_subproject}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/
    COMPONENT Libraries
  )
ENDIF (ANTIQUA_LSB_INSTALL)

