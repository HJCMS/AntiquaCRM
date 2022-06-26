# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "DeliverService")

SET (subproject_LINKER
 AntiquaCore
 Settings
 SqlCore
)

SET (subproject_SOURCES
 deliveritem.cpp
 deliverservicedialog.cpp
 deliverservicelist.cpp
 deliverservice.cpp
)

SET (subproject_HEADERS
 deliveritem.h
 deliverservicedialog.h
 deliverservicelist.h
 deliverservice.h
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
