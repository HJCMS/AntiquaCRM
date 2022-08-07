# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Utils")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 AntiquaCore
 Autofill
)

SET (subproject_SOURCES
  applicationclient.cpp
  serialid.cpp
  boolbox.cpp
  conditionfield.cpp
  datetimeedit.cpp
  ecurrency.cpp
  editoractionbar.cpp
  editormain.cpp
  emailedit.cpp
  eucountries.cpp
  filedialog.cpp
  genderbox.cpp
  groupbox.cpp
  intspinbox.cpp
  inventory.cpp
  searchfilter.cpp
  isbnedit.cpp
  lineedit.cpp
  messagebox.cpp
  orderstatusbox.cpp
  orderspaymentbox.cpp
  phoneedit.cpp
  postalcode.cpp
  priceedit.cpp
  salutationbox.cpp
  searchbar.cpp
  sizeedit.cpp
  setlanguage.cpp
  statsactionbar.cpp
  storageedit.cpp
  strlineedit.cpp
  techniqueedit.cpp
  textfield.cpp
  yearedit.cpp
  utilsmain.cpp
  trustbox.cpp
  myicontheme.cpp
)

SET (subproject_HEADERS
  applicationclient.h
  serialid.h
  boolbox.h
  conditionfield.h
  datetimeedit.h
  editoractionbar.h
  emailedit.h
  ecurrency.h
  editormain.h
  eucountries.h
  filedialog.h
  genderbox.h
  groupbox.h
  intspinbox.h
  inventory.h
  searchfilter.h
  isbnedit.h
  lineedit.h
  messagebox.h
  orderstatusbox.h
  orderspaymentbox.h
  phoneedit.h
  postalcode.h
  priceedit.h
  salutationbox.h
  searchbar.h
  sizeedit.h
  setlanguage.h
  statsactionbar.h
  storageedit.h
  strlineedit.h
  techniqueedit.h
  textfield.h
  yearedit.h
  utilsmain.h
  trustbox.h
  myicontheme.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  UtilsMain
  EditorMain
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
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${subproject_LINKER}
)

