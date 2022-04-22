# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Inventory")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
 articleid.cpp
 boolbox.cpp
 editoractionbar.cpp
 filedialog.cpp
 imagetoolbar.cpp
 imagewidget.cpp
 intspinbox.cpp
 isbnedit.cpp
 messagebox.cpp
 priceedit.cpp
 searchbar.cpp
 setlanguage.cpp
 statsactionbar.cpp
 storageedit.cpp
 strlineedit.cpp
 techniqueedit.cpp
 textfield.cpp
 yearedit.cpp
)

SET (subproject_HEADERS
 articleid.h
 boolbox.h
 editoractionbar.h
 filedialog.h
 imagetoolbar.h
 imagewidget.h
 intspinbox.h
 isbnedit.h
 messagebox.h
 priceedit.h
 searchbar.h
 setlanguage.h
 statsactionbar.h
 storageedit.h
 strlineedit.h
 techniqueedit.h
 textfield.h
 yearedit.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Sql
)

