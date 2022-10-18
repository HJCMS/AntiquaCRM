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

SET (projectLibrary_LINKER
 AntiquaCRM
)

SET (projectLibrary_FILES
  synopsis_h.sh
)

SET (projectLibrary_SOURCES
 antiquacombobox.cpp
 antiquadateinfo.cpp
 antiquadoublebox.cpp
 antiquailabel.cpp
 antiqualineedit.cpp
 antiquaspinbox.cpp
 bookbinding.cpp
 boolbox.cpp
 conditionedit.cpp
 ecurrency.cpp
 inputedit.cpp
 intspinbox.cpp
 inventory.cpp
 inventoryeditor.cpp
 inventorytable.cpp
 isbnedit.cpp
 language.cpp
 lineedit.cpp
 postalcodeedit.cpp
 priceedit.cpp
 searchfilter.cpp
 searchlineedit.cpp
 selecteucountry.cpp
 serialid.cpp
 sizeedit.cpp
 storagelocation.cpp
 tabsearchbar.cpp
 tabstatusbar.cpp
 textfield.cpp
 yearedit.cpp
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
 bookbinding.h
 boolbox.h
 conditionedit.h
 ecurrency.h
 inputedit.h
 intspinbox.h
 inventoryeditor.h
 inventory.h
 inventorytable.h
 isbnedit.h
 language.h
 lineedit.h
 postalcodeedit.h
 priceedit.h
 searchfilter.h
 searchlineedit.h
 selecteucountry.h
 serialid.h
 sizeedit.h
 storagelocation.h
 tabsearchbar.h
 tabstatusbar.h
 textfield.h
 yearedit.h
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
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Widgets
  ${projectLibrary_LINKER}
)
