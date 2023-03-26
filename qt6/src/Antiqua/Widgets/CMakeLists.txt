# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_subProjectLibrary "AntiquaWidgets")

ADD_SUBDIRECTORY(private)
ADD_SUBDIRECTORY(templates)

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
## Inputs
 abstractinput.cpp
 alabel.cpp
 acombobox.cpp
 adatetimeedit.cpp
 adoublebox.cpp
 alineedit.cpp
 aspinbox.cpp
 aslider.cpp
## Widgets
 whatsthisbutton.cpp
 bookbindingedit.cpp
 crowdedit.cpp
 conditionedit.cpp
 emailedit.cpp
 genderedit.cpp
 priceedit.cpp
 phoneedit.cpp
 postalcodeedit.cpp
 salutationedit.cpp
)

SET (_library_HEADERS
## Inputs
 abstractinput.h
 alabel.h
 acombobox.h
 adatetimeedit.h
 adoublebox.h
 alineedit.h
 aspinbox.h
 aslider.h
 AntiquaInput
## Widgets
 AntiquaWidgets
 whatsthisbutton.h
 bookbindingedit.h
 crowdedit.h
 conditionedit.h
 emailedit.h
 genderedit.h
 priceedit.h
 phoneedit.h
 postalcodeedit.h
 salutationedit.h
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
