# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (${SUB_PROJECT_INCLUDE_DIRS})

SET (_sub_target_FILES
 buchfreund.cpp
 buchfreund.h
)

ADD_LIBRARY (AntiquacmdBuchfreund STATIC
  ${_sub_target_FILES}
)

SET_TARGET_PROPERTIES (AntiquacmdBuchfreund PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME AntiquacmdBuchfreund
)

TARGET_LINK_LIBRARIES (AntiquacmdBuchfreund PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Xml
  Qt${QT_VERSION_MAJOR}::Sql
  AntiquaCRM
)
