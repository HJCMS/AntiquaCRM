# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (${SUB_PROJECT_INCLUDE_DIRS})

SET (_sub_target_FILES
 abebooks.cpp
 abebooksdocument.cpp
 abebooks.h
 abebooksdocument.h
)

ADD_LIBRARY (AntiquacmdAbeBooks STATIC
  ${_sub_target_FILES}
)

SET_TARGET_PROPERTIES (AntiquacmdAbeBooks PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME AntiquacmdAbeBooks
)

TARGET_LINK_LIBRARIES (AntiquacmdAbeBooks PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Xml
  Qt${QT_VERSION_MAJOR}::Sql
  AntiquaCRM
)
