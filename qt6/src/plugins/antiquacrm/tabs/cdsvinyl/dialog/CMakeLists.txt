# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_cdtracking "CDTracking")

SET (cdtrack_SOURCES
 selectgenre.cpp
 cdreaddialog.cpp
 cddiscid.cpp
 discinfo.cpp
 trackslistwidget.cpp
)

SET (cdtrack_HEADERS
 selectgenre.h
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
  POSITION_INDEPENDENT_CODE ON
)

TARGET_LINK_LIBRARIES (${_cdtracking} PRIVATE
  Qt6::Core
  Qt6::Widgets
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  ${CDTRACKING_LIBRARIES}
  AntiquaCRM
  AntiquaWidgets
)
