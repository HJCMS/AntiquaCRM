# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaPurchaseTable")

SET (subproject_LINKER
 AntiquaCRM
)

SET (subproject_SOURCES
 purchasetable.cpp
 purchasetablemodel.cpp
 purchasetabledelegate.cpp
)

SET (subproject_HEADERS
 purchasetable.h
 purchasetablemodel.h
 purchasetabledelegate.h
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
  ${subproject_LINKER}
)
