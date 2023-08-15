# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_sharedLibrary "AntiquaWidgets")

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_SUBDIRECTORY (private)

SET (_library_FILES
  Widgets.dox
  antiquabuttons.h.cmake
  antiquainput.h.cmake
  antiquamail.h.cmake
  antiquawidgets.h.cmake
)

CONFIGURE_FILE (antiquawidgets.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquawidgets.h
)

CONFIGURE_FILE (antiquainput.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquainput.h
)

CONFIGURE_FILE (antiquabuttons.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquabuttons.h
)

CONFIGURE_FILE (antiquamail.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquamail.h
)

SET (_library_config_HEADERS
  antiquabuttons.h
  antiquainput.h
  antiquamail.h
  antiquawidgets.h
)

SET (_library_config_TEMPLATES
  AntiquaInput
  AntiquaWidgets
  AntiquaButtons
  AntiquaMail
)

SET (_library_SOURCES
  acolorluminance.cpp
  acombobox.cpp
  adatetimeedit.cpp
  adoublebox.cpp
  ainputwidget.cpp
  alabel.cpp
  alineedit.cpp
  apluginconfigwidget.cpp
  aslider.cpp
  aspinbox.cpp
  atextedit.cpp
  awhatsthisbutton.cpp
  bookbindingedit.cpp
  boolbox.cpp
  conditionedit.cpp
  configgridlayout.cpp
  crowdedit.cpp
  datetimeinfo.cpp
  descripeeditor.cpp
  emailedit.cpp
  genderedit.cpp
  groupboxedit.cpp
  imagethumbnail.cpp
  imagetoolbar.cpp
  isbnedit.cpp
  keywordsedit.cpp
  mailbutton.cpp
  maildialog.cpp
  numedit.cpp
  printerbutton.cpp
  phoneedit.cpp
  postalcodeedit.cpp
  postalcodelocation.cpp
  postalcodemodel.cpp
  postalcodestate.cpp
  priceedit.cpp
  salutationedit.cpp
  selectcurrency.cpp
  selectdeliverpackage.cpp
  selectdeliverservice.cpp
  selecteucountry.cpp
  selectfile.cpp
  selectlanguage.cpp
  selectmediatype.cpp
  selectorderpayment.cpp
  selectorderstatus.cpp
  selectpaymentmethod.cpp
  selectpaymentstatus.cpp
  selectstorage.cpp
  selecttargets.cpp
  selectvat.cpp
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
  acolorluminance.h
  acombobox.h
  adatetimeedit.h
  adoublebox.h
  ainputwidget.h
  alabel.h
  alineedit.h
  antiquaicon.h
  apluginconfigwidget.h
  aslider.h
  aspinbox.h
  atextedit.h
  awhatsthisbutton.h
  bookbindingedit.h
  boolbox.h
  conditionedit.h
  configgridlayout.h
  crowdedit.h
  datetimeinfo.h
  descripeeditor.h
  emailedit.h
  genderedit.h
  groupboxedit.h
  imagethumbnail.h
  imagetoolbar.h
  isbnedit.h
  keywordsedit.h
  mailbutton.h
  maildialog.h
  numedit.h
  printerbutton.h
  phoneedit.h
  postalcodeedit.h
  postalcodelocation.h
  postalcodemodel.h
  postalcodestate.h
  priceedit.h
  salutationedit.h
  selectcurrency.h
  selectdeliverpackage.h
  selectdeliverservice.h
  selecteucountry.h
  selectfile.h
  selectlanguage.h
  selectmediatype.h
  selectorderpayment.h
  selectorderstatus.h
  selectpaymentmethod.h
  selectpaymentstatus.h
  selectstorage.h
  selecttargets.h
  selectvat.h
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
 AntiquaCRM
 AntiquaSubWidgets
)

INSTALL (TARGETS ${_sharedLibrary}
 LIBRARY DESTINATION lib${LIB_SUFFIX}/
 COMPONENT Libraries
 NAMELINK_COMPONENT Development
)

INSTALL (FILES ${library_HEADERS} ${_stl_HEADERS}
 DESTINATION include/AntiquaCRM
 COMPONENT Development
)

##EOF
