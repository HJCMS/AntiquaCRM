# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_sharedLibrary "AntiquaWidgets")

ADD_SUBDIRECTORY (templates)

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_SUBDIRECTORY (private)

CONFIGURE_FILE (AntiquaWidgets.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaWidgets
)

SET (_library_FILES
 AntiquaInput
 AntiquaInput.cmake
 AntiquaWidgets
 AntiquaWidgets.cmake
)

SET (_library_SOURCES
 abstractinput.cpp
 acolorluminance.cpp
 acombobox.cpp
 adatetimeedit.cpp
 adoublebox.cpp
 alabel.cpp
 alineedit.cpp
 aslider.cpp
 aspinbox.cpp
 awhatsthisbutton.cpp
 bookbindingedit.cpp
 boolbox.cpp
 conditionedit.cpp
 crowdedit.cpp
 emailedit.cpp
 genderedit.cpp
 imagefilesource.cpp
 imagethumbnail.cpp
 imagetoolbar.cpp
 imageviewer.cpp
 isbnedit.cpp
 keywordsedit.cpp
 numedit.cpp
 phoneedit.cpp
 postalcodeedit.cpp
 priceedit.cpp
 salutationedit.cpp
 selecteucountry.cpp
 selectlanguage.cpp
 selectpaymentmethod.cpp
 selectpaymentstatus.cpp
 selectstorage.cpp
 serialid.cpp
 splitter.cpp
 sqlinfopopup.cpp
 tablecontextmenu.cpp
 tableheader.cpp
 tableview.cpp
 textfield.cpp
 textline.cpp
 yearedit.cpp
)

SET (_library_HEADERS
 abstractinput.h
 acolorluminance.h
 acombobox.h
 adatetimeedit.h
 adoublebox.h
 alabel.h
 alineedit.h
 aslider.h
 aspinbox.h
 awhatsthisbutton.h
 bookbindingedit.h
 boolbox.h
 conditionedit.h
 crowdedit.h
 emailedit.h
 genderedit.h
 imagefilesource.h
 imagethumbnail.h
 imagetoolbar.h
 imageviewer.h
 isbnedit.h
 keywordsedit.h
 numedit.h
 phoneedit.h
 postalcodeedit.h
 priceedit.h
 salutationedit.h
 selecteucountry.h
 selectlanguage.h
 selectpaymentmethod.h
 selectpaymentstatus.h
 selectstorage.h
 serialid.h
 splitter.h
 sqlinfopopup.h
 tablecontextmenu.h
 tableheader.h
 tableview.h
 textfield.h
 textline.h
 yearedit.h
)

ADD_LIBRARY (${_sharedLibrary} SHARED
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
 AntiquaCRM
 AntiquaSubWidgets
)

INSTALL (TARGETS ${_sharedLibrary}
 LIBRARY DESTINATION lib${LIB_SUFFIX}/
 COMPONENT Libraries
 NAMELINK_COMPONENT Development
)

INSTALL (FILES ${library_HEADERS} AntiquaInput AntiquaWidgets
 DESTINATION include/AntiquaCRM
 COMPONENT Development
)

##EOF
