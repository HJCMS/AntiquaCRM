# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaCompanyData")

SET (subproject_SOURCES
 companydata.cpp
 companywidget.cpp
)

SET (subproject_HEADERS
 companydata.h
 companywidget.h
)

INCLUDE_DIRECTORIES (
 ${PROJECT_INCLUDE_DIRS}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY (${_subproject} STATIC
 ${subproject_HEADERS}
 ${subproject_SOURCES}
 ${subproject_FILES}
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
