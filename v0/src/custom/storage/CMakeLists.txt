# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "StorageLocation")

SET (subproject_LINKER
 SqlCore
 Utils
)

SET (subproject_SOURCES
  storagelocation.cpp
  storagetable.cpp
  storagemodel.cpp
  storagecategory.cpp
  storageeditor.cpp
)

SET (subproject_HEADERS
  storagelocation.h
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
