# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Utils")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_SOURCES
  serialid.cpp
  boolbox.cpp
  editoractionbar.cpp
  emailedit.cpp
  filedialog.cpp
  genderbox.cpp
  imagetoolbar.cpp
  imagewidget.cpp
  intspinbox.cpp
  inventory.cpp
  isbnedit.cpp
  lineedit.cpp
  messagebox.cpp
  phoneedit.cpp
  postalcode.cpp
  priceedit.cpp
  salutationbox.cpp
  searchbar.cpp
  setlanguage.cpp
  statsactionbar.cpp
  storageedit.cpp
  strlineedit.cpp
  techniqueedit.cpp
  textfield.cpp
  yearedit.cpp
  utilsmain.cpp
)

SET (subproject_HEADERS
  serialid.h
  boolbox.h
  editoractionbar.h
  emailedit.h
  filedialog.h
  genderbox.h
  imagetoolbar.h
  imagewidget.h
  intspinbox.h
  inventory.h
  isbnedit.h
  lineedit.h
  messagebox.h
  phoneedit.h
  postalcode.h
  priceedit.h
  salutationbox.h
  searchbar.h
  setlanguage.h
  statsactionbar.h
  storageedit.h
  strlineedit.h
  techniqueedit.h
  textfield.h
  yearedit.h
  utilsmain.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  UtilsMain
  Utils
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

