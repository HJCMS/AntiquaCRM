# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "StoragesDialog")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_SUBDIRECTORY(xml)

QT6_ADD_RESOURCES (_library_RC_FILES categories.qrc)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (${_library} STATIC
  ${_library_RC_FILES} storageutil.h
  storagesdialog.h storagesdialog.cpp
  storagetable.h storagetable.cpp
  storagetablemodel.h storagetablemodel.cpp
  storageeditorwidget.h storageeditorwidget.cpp
  storagecategory.h storagecategory.cpp
  storagesearchbar.h storagesearchbar.cpp
)

SET_TARGET_PROPERTIES (${_library} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_library}
)

TARGET_LINK_LIBRARIES (${_library} PRIVATE
 Qt6::Core
 Qt6::Widgets
 Qt6::Sql
 Qt6::Network
 Qt6::Xml
 AntiquaCRM
 AntiquaWidgets
)

##EOF
