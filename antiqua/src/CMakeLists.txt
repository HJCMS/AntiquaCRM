## Sources

SET (ANTIQUA_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}/settings
  ${CMAKE_CURRENT_SOURCE_DIR}/settings
  ${CMAKE_CURRENT_BINARY_DIR}/plugins
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins
)

ADD_SUBDIRECTORY (settings)
ADD_SUBDIRECTORY (plugins)

SET (PROJECT_SOURCES
  main.cpp
  antiqua.cpp
  antiquamainwindow.cpp
  antiqua.h
  antiquamainwindow.h
  antiquamainwindow.ui
  ${TS_FILES}
)

INCLUDE_DIRECTORIES (${ANTIQUA_PROJECT_INCULDE})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(antiqua
      MANUAL_FINALIZATION
      ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(antiqua
      ${PROJECT_SOURCES}
    )
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(antiqua PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties(antiqua PROPERTIES
  WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(antiqua)
endif()
