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
 ${QRENCODE_INCLUDE_DIRS}
)

ADD_SUBDIRECTORY (private)

SET (_library_FILES
 Widgets.dox
 antiquainput.h.cmake
 antiquawidgets.h.cmake
 antiquabuttons.h.cmake
 antiquaprinting.h.cmake
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

CONFIGURE_FILE (antiquaprinting.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquaprinting.h
)

SET (_stl_HEADERS
 AntiquaInput
 antiquainput.h
 AntiquaWidgets
 antiquawidgets.h
 AntiquaButtons
 antiquabuttons.h
 AntiquaPrintDialog
 AntiquaPrinting
 antiquaprinting.h
)

SET (_library_SOURCES
  ainputwidget.cpp
  acolorluminance.cpp
  acombobox.cpp
  adatetimeedit.cpp
  adoublebox.cpp
  alabel.cpp
  alineedit.cpp
  aprintdialog.cpp
  aprintingpage.cpp
  aslider.cpp
  aspinbox.cpp
  atextedit.cpp
  awhatsthisbutton.cpp
  bookbindingedit.cpp
  boolbox.cpp
  conditionedit.cpp
  crowdedit.cpp
  datetimeinfo.cpp
  descripeeditor.cpp
  emailedit.cpp
  genderedit.cpp
  imagefilesource.cpp
  imageimportdialog.cpp
  imagethumbnail.cpp
  imagetoolbar.cpp
  imageviewer.cpp
  isbnedit.cpp
  keywordsedit.cpp
  mailbutton.cpp
  numedit.cpp
  phoneedit.cpp
  postalcodeedit.cpp
  priceedit.cpp
  printbookcard.cpp
  printdeliverynote.cpp
  printerbutton.cpp
  printinvoice.cpp
  salutationedit.cpp
  selectcurrency.cpp
  selecteucountry.cpp
  selectfile.cpp
  selectlanguage.cpp
  selectmediatype.cpp
  selectpaymentmethod.cpp
  selectpaymentstatus.cpp
  selectstorage.cpp
  selecttargets.cpp
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
  ainputwidget.h
  acolorluminance.h
  acombobox.h
  adatetimeedit.h
  adoublebox.h
  alabel.h
  alineedit.h
  antiquabuttons.h
  antiquainput.h
  antiquaprinting.h
  antiquawidgets.h
  aprintdialog.h
  aprintingpage.h
  aslider.h
  aspinbox.h
  atextedit.h
  awhatsthisbutton.h
  bookbindingedit.h
  boolbox.h
  conditionedit.h
  crowdedit.h
  datetimeinfo.h
  descripeeditor.h
  emailedit.h
  genderedit.h
  imagefilesource.h
  imageimportdialog.h
  imagethumbnail.h
  imagetoolbar.h
  imageviewer.h
  isbnedit.h
  keywordsedit.h
  mailbutton.h
  numedit.h
  phoneedit.h
  postalcodeedit.h
  priceedit.h
  printbookcard.h
  printdeliverynote.h
  printerbutton.h
  printinvoice.h
  salutationedit.h
  selectcurrency.h
  selecteucountry.h
  selectfile.h
  selectlanguage.h
  selectmediatype.h
  selectpaymentmethod.h
  selectpaymentstatus.h
  selectstorage.h
  selecttargets.h
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
 ${_stl_HEADERS}
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
