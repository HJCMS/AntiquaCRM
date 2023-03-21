# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_subProjectLibrary "AntiquaWidgets")

ADD_SUBDIRECTORY(private)

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

CONFIGURE_FILE (AntiquaWidgets.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaWidgets
)

SET (_library_FILES
 private/abstractinput_p.h
 AntiquaWidgets.cmake
)

SET (_library_SOURCES
 abstractinput.cpp
 alabel.cpp
 acombobox.cpp
 adatetimeedit.cpp
 alineedit.cpp
 aspinbox.cpp
 adoublebox.cpp
## Widgets
 whatsthisbutton.cpp
 priceedit.cpp
)

SET (_library_HEADERS
 abstractinput.h
 alabel.h
 acombobox.h
 adatetimeedit.h
 alineedit.h
 aspinbox.h
 adoublebox.h
 AntiquaWidgets
 AntiquaInput
## Widgets
 whatsthisbutton.h
 priceedit.h
)

ADD_LIBRARY (${_subProjectLibrary} SHARED
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_library_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_subProjectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_subProjectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_subProjectLibrary} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Network
  Qt6::Sql
  Qt6::Xml
  AntiquaCRM
)

INSTALL (TARGETS ${_subProjectLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${library_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)

#EOF
