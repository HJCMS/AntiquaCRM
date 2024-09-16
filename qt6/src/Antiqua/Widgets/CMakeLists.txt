# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_sharedLibrary "AntiquaWidgets")

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

## ADD_SUBDIRECTORY (private)

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
  adialog.cpp
  adoublebox.cpp
  ainputwidget.cpp
  alabel.cpp
  alineedit.cpp
  anavigationbar.cpp
  amenu.cpp
  apluginconfigwidget.cpp
  apopupmessage.cpp
  aslider.cpp
  aspinbox.cpp
  atextedit.cpp
  awhatsthisbutton.cpp
  bookbindingedit.cpp
  boolbox.cpp
  conditionedit.cpp
  configgridlayout.cpp
  crowdedit.cpp
  customersdatawidget.cpp
  datetimeinfo.cpp
  descripeeditor.cpp
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
  urilineedit.cpp
  yearedit.cpp
)

SET (_library_HEADERS
  acolorluminance.h
  acombobox.h
  adatetimeedit.h
  adialog.h
  adoublebox.h
  ainputwidget.h
  alabel.h
  alineedit.h
  anavigationbar.h
  antiquaicon.h
  apluginconfigwidget.h
  amenu.h
  apopupmessage.h
  aslider.h
  aspinbox.h
  atextedit.h
  awhatsthisbutton.h
  bookbindingedit.h
  boolbox.h
  conditionedit.h
  configgridlayout.h
  crowdedit.h
  customersdatawidget.h
  datetimeinfo.h
  descripeeditor.h
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
  urilineedit.h
  yearedit.h
)

SET (_sub_widgets_source
 private/lineinputvalidator_p.h private/lineinputvalidator.cpp
 private/keywordlistview_p.h private/keywordlistview.cpp
 private/keywordlabel_p.h private/keywordlabel.cpp
 private/mailtemplatekeys_p.h private/mailtemplatekeys.cpp
 private/mailcommand_p.h private/mailcommand.cpp
 private/mailattachments_p.h private/mailattachments.cpp
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${_library_config_HEADERS}
 ${_library_config_TEMPLATES}
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_sub_widgets_source}
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
