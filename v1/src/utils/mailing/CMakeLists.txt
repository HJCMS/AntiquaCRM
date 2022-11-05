# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaMailing")

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
 AntiquaTemplates
)

SET (subproject_FILES
 README.md
)

SET (subproject_SOURCES
 mailattachments.cpp
 mailforwarddialog.cpp
 mailbody.cpp
 mailtemplatekeys.cpp
 mailcommand.cpp
)

SET (subproject_HEADERS
 mailattachments.h
 mailforwarddialog.h
 mailbody.h
 mailtemplatekeys.h
 mailcommand.h
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
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Sql
  ${subproject_LINKER}
)
