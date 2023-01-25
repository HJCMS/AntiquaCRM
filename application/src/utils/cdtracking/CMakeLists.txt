# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

IF (CDTRACKING_SUPPORT)

SET(_cdtracking "AntiquaCDTracking")

INCLUDE_DIRECTORIES (
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${PROJECT_INCLUDE_DIRS}
)

SET (cdtrack_SOURCES
 cdreaddialog.cpp
 cddiscid.cpp
 discinfo.cpp
 trackslistwidget.cpp
)

SET (cdtrack_HEADERS
 cdreaddialog.h
 cddiscid.h
 discinfo.h
 trackslistwidget.h
)

ADD_LIBRARY (${_cdtracking} STATIC
 ${cdtrack_SOURCES}
 ${cdtrack_HEADERS}
)

SET_TARGET_PROPERTIES (${_cdtracking} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_cdtracking}
)

TARGET_LINK_LIBRARIES (${_cdtracking} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  ${CDTRACKING_LIBRARIES}
  AntiquaCRM
  AntiquaWidgets
)

ENDIF (CDTRACKING_SUPPORT)
