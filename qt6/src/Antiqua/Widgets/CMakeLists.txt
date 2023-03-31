# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_subProjectLibrary "AntiquaWidgets")

ADD_SUBDIRECTORY (icons)
ADD_SUBDIRECTORY (private)
ADD_SUBDIRECTORY (templates)

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

CONFIGURE_FILE (AntiquaWidgets.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaWidgets
)

# Section: QRessource
# BEGIN:QRC
SET (ICON_RESOURCE_PATH
 ${CMAKE_CURRENT_BINARY_DIR}/AntiquaWidgets.rcc
)
QT_ADD_BINARY_RESOURCES(_library_Resources
 ${CMAKE_CURRENT_SOURCE_DIR}/iconset.qrc
 DESTINATION ${ICON_RESOURCE_PATH}
)
ADD_DEFINITIONS (-DANTIQUACRM_RESOURCE_PATH="${ICON_RESOURCE_PATH}")
# END:QRC

SET (_library_FILES
 private/abstractinput_p.h
 AntiquaInput.cmake
 AntiquaWidgets.cmake
)

SET (_library_SOURCES
## Inputs
 abstractinput.cpp
 aguiutils.cpp
 alabel.cpp
 acombobox.cpp
 adatetimeedit.cpp
 adoublebox.cpp
 alineedit.cpp
 aspinbox.cpp
 aslider.cpp
 colorluminance.cpp
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
 selectlanguage.cpp
 selectpaymentmethod.cpp
 selectpaymentstatus.cpp
 sqlinfopopup.cpp
 tablecontextmenu.cpp
)

SET (_library_HEADERS
## Inputs
 abstractinput.h
 aguiutils.h
 alabel.h
 acombobox.h
 adatetimeedit.h
 adoublebox.h
 alineedit.h
 aspinbox.h
 aslider.h
 colorluminance.h
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
 selectlanguage.h
 selectpaymentmethod.h
 selectpaymentstatus.h
 sqlinfopopup.h
 tablecontextmenu.h
)

ADD_LIBRARY (${_subProjectLibrary} SHARED
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_library_FILES}
 ${_library_Resources}
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
