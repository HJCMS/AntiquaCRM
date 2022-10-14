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
 antiquaspinbox.cpp
 antiquailabel.cpp
 conditionedit.cpp
 inventory.cpp
 inventorytable.cpp
 inputedit.cpp
 bookbinding.cpp
 postalcodeedit.cpp
 storagelocation.cpp
 selecteucountry.cpp
 searchfilter.cpp
 searchbar.cpp
 searchlineedit.cpp
)

SET (projectLibrary_HEADERS
 AntiquaInputEdit
 AntiquaWidgets
 antiquacombobox.h
 antiquadateinfo.h
 antiquadoublebox.h
 antiquaspinbox.h
 antiquailabel.h
 conditionedit.h
 inventory.h
 inventorytable.h
 inputedit.h
 bookbinding.h
 postalcodeedit.h
 storagelocation.h
 selecteucountry.h
 searchfilter.h
 searchbar.h
 searchlineedit.h
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
