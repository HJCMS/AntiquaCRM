# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_sharedLibrary "AntiquaPrinting")

INCLUDE_DIRECTORIES (
 ${INTERNAL_LIBRARY_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QRENCODE_INCLUDE_DIRS}
)

SET (_library_FILES
  Printing.dox
  antiquaprinting.h.cmake
  antiquaprintdialog.h.cmake
  antiquaprintsupport.h.cmake
)

CONFIGURE_FILE (antiquaprinting.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquaprinting.h
)

CONFIGURE_FILE (antiquaprintdialog.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquaprintdialog.h
)

CONFIGURE_FILE (antiquaprintsupport.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquaprintsupport.h
)

SET (_library_config_HEADERS
  antiquaprinting.h
  antiquaprintdialog.h
  antiquaprintsupport.h
)

SET (_library_config_TEMPLATES
 AntiquaPrintDialog
 AntiquaPrintSupport
 AntiquaPrinting
)

SET (_library_SOURCES
  aprintdialog.cpp
  aprintingbody.cpp
  aprintingpage.cpp
  aprinttools.cpp
  bookcardqrcode.cpp
  bookcardpage.cpp
  printbookcard.cpp
  printdeliverynote.cpp
  printinvoice.cpp
  printrefund.cpp
  printreminder.cpp
  printadmonition.cpp
  printreport.cpp
)

SET (_library_HEADERS
  aprintingbody.h
  aprintingpage.h
  aprinttools.h
  aprintdialog.h
  bookcardqrcode.h
  bookcardpage.h
  printbookcard.h
  printdeliverynote.h
  printinvoice.h
  printrefund.h
  printreminder.h
  printadmonition.h
  printreport.h
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${_library_config_HEADERS}
 ${_library_config_TEMPLATES}
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_library_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_sharedLibrary} PROPERTIES
 VERSION ${ANTIQUACRM_VERSION_STRING}
 SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
 OUTPUT_NAME ${_sharedLibrary}
 LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_sharedLibrary} PRIVATE
 Qt6::Core
 Qt6::Gui
 Qt6::Widgets
 Qt6::Network
 Qt6::Sql
 Qt6::Xml
 Qt6::PrintSupport
 ${QRENCODE_LIBRARIES}
 AntiquaCRM
 AntiquaWidgets
)

IF (CMAKE_HOST_LINUX)
  INSTALL (TARGETS ${_sharedLibrary}
   LIBRARY DESTINATION lib${LIB_SUFFIX}/
   COMPONENT Libraries
   NAMELINK_COMPONENT Development
  )

  INSTALL (FILES ${library_HEADERS} ${_stl_HEADERS}
   DESTINATION include/AntiquaCRM
   COMPONENT Development
  )
ENDIF()

##EOF
