# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

ADD_SUBDIRECTORY(templates)

SET(_projectLibrary "AntiquaWidgets")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

CONFIGURE_FILE (AntiquaWidgets.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaWidgets
)

CONFIGURE_FILE (AntiquaInputEdit.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaInputEdit
)

SET (projectLibrary_FILES
  synopsis_h.sh
  widgets.dox
)

SET (projectLibrary_SOURCES
 antiquacombobox.cpp
 antiquadateinfo.cpp
 antiquadoublebox.cpp
 antiquailabel.cpp
 antiqualineedit.cpp
 antiquaspinbox.cpp
 antiquasplitter.cpp
 antiquatabbar.cpp
 bookbinding.cpp
 addressinfo.cpp
 boolbox.cpp
 conditionedit.cpp
 deliveryid.cpp
 descripeedit.cpp
 dnbsearch.cpp
 opacityslider.cpp
 ecurrency.cpp
 editoractionbar.cpp
 editinfolabel.cpp
 editortab.cpp
 emailedit.cpp
 genderbox.cpp
 groupbox.cpp
 inputedit.cpp
 intspinbox.cpp
 inventory.cpp
 inventoryeditor.cpp
 inventorytable.cpp
 isbnedit.cpp
 language.cpp
 lineedit.cpp
 mailbutton.cpp
 orderstatusselecter.cpp
 orderpaymentstatusselecter.cpp
 paymentmethodselecter.cpp
 providerpaymentsselecter.cpp
 phoneedit.cpp
 postalcodeedit.cpp
 priceedit.cpp
 printerbutton.cpp
 salutationbox.cpp
 searchfilter.cpp
 searchlineedit.cpp
 selecteucountry.cpp
 selectarticletype.cpp
 selectmediatype.cpp
 serialid.cpp
 showpaymentmethod.cpp
 sizeedit.cpp
 storagelocation.cpp
 tablecontextmenu.cpp
 tabsearchbar.cpp
 tabstatusbar.cpp
 taxlevelbox.cpp
 techniqueedit.cpp
 textfield.cpp
 trustedbox.cpp
 yearedit.cpp
 yearselect.cpp
)

SET (projectLibrary_HEADERS
 AntiquaInputEdit
 AntiquaWidgets
 antiquacombobox.h
 antiquadateinfo.h
 antiquadoublebox.h
 antiquailabel.h
 antiqualineedit.h
 antiquaspinbox.h
 antiquasplitter.h
 antiquatabbar.h
 addressinfo.h
 bookbinding.h
 boolbox.h
 conditionedit.h
 deliveryid.h
 descripeedit.h
 dnbsearch.h
 opacityslider.h
 ecurrency.h
 editoractionbar.h
 editinfolabel.h
 editortab.h
 emailedit.h
 genderbox.h
 groupbox.h
 inputedit.h
 intspinbox.h
 inventoryeditor.h
 inventory.h
 inventorytable.h
 isbnedit.h
 language.h
 lineedit.h
 mailbutton.h
 orderstatusselecter.h
 orderpaymentstatusselecter.h
 paymentmethodselecter.h
 providerpaymentsselecter.h
 phoneedit.h
 postalcodeedit.h
 priceedit.h
 printerbutton.h
 salutationbox.h
 searchfilter.h
 searchlineedit.h
 selecteucountry.h
 selectarticletype.h
 selectmediatype.h
 serialid.h
 showpaymentmethod.h
 sizeedit.h
 storagelocation.h
 tablecontextmenu.h
 tabsearchbar.h
 tabstatusbar.h
 taxlevelbox.h
 techniqueedit.h
 textfield.h
 trustedbox.h
 yearedit.h
 yearselect.h
)

ADD_LIBRARY (${_projectLibrary} STATIC
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_projectLibrary}
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  AntiquaCRM
)
