# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaStorage")

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
)

QT5_ADD_RESOURCES (STORAGE_RC_FILES categories.qrc)

SET (subproject_SOURCES
 storageeditdialog.cpp
 storagetable.cpp
 storagemodel.cpp
 storagecategory.cpp
 storageeditor.cpp
 ${STORAGE_RC_FILES}
)

SET (subproject_HEADERS
 storageeditdialog.h
 storagetable.h
 storagemodel.h
 storagecategory.h
 storageeditor.h
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
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${subproject_LINKER}
)
