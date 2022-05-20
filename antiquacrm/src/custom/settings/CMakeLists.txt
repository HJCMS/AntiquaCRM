SET(_subproject "Settings")

SET (subproject_LINKER
 AntiquaCore
 Utils
)

SET (subproject_FILES
  README
  TODO
)

SET (subproject_SOURCES
 applsettings.cpp
 configdialog.cpp
 generalsettings.cpp
 pgsqlsettings.cpp
 settingswidget.cpp
 viewsettings.cpp
 companysettings.cpp
 printsettings.cpp
 providersettings.cpp
)

SET (subproject_HEADERS
 applsettings.h
 configdialog.h
 generalsettings.h
 pgsqlsettings.h
 settingswidget.h
 viewsettings.h
 companysettings.h
 printsettings.h
 providersettings.h
 SettingsDialog
)

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${INCLUDE_CONFIG_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
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
  Qt${QT_VERSION_MAJOR}::Network
  ${subproject_LINKER}
)
