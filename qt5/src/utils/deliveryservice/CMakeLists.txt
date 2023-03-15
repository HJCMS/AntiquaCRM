# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "DeliveryService")

SET (subproject_SOURCES
 deliveritem.cpp
 deliverpackagebox.cpp
 deliverservicebox.cpp
 deliverservice.cpp
 deliverservicedialog.cpp
 deliverserviceedit.cpp
 deliverservicelist.cpp
)

SET (subproject_HEADERS
 deliveritem.h
 deliverpackagebox.h
 deliverservicebox.h
 deliverservicedialog.h
 deliverserviceedit.h
 deliverservice.h
 deliverservicelist.h
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
 AntiquaCRM
 AntiquaWidgets
)
