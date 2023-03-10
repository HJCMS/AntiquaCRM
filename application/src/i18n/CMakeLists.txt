# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

## SUBDIRECTORY(qt5-tr)
## WARNING qt5-tr/translations isn't a part of the default Qt5 Installation.
## https://github.com/HJCMS/AntiquaCRM/blob/main/INSTALL.md
## https://doc.qt.io/qt-5/internationalization.html

## Add supported translation language here ...
SET(supported_languages
 de
)

EXECUTE_PROCESS (COMMAND ${QT_QMAKE_EXECUTABLE}
 -query QT_INSTALL_BINS OUTPUT_VARIABLE QT_BINARY_TARGET)

FIND_PROGRAM (LRELEASE_EXECUTABLE
 "lrelease" "lrelease-qt5" "qt5-lrelease"
 PATHS ${QT_BINARY_TARGET})

SET(LRELEASE_OPTIONS -silent -compress -nounfinished -removeidentical)

FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
FOREACH (TR_LANGUAGE ${supported_languages})
 MESSAGE (STATUS "Generate ${PROJECTNAME}_${TR_LANGUAGE}.qm translation.")
 EXECUTE_PROCESS (COMMAND ${LRELEASE_EXECUTABLE} ${LRELEASE_OPTIONS}
    ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm_${TR_LANGUAGE}.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/qt5-tr/translations/qtbase_${TR_LANGUAGE}.ts
    -qm ${CMAKE_CURRENT_BINARY_DIR}/${PROJECTNAME}_${TR_LANGUAGE}.qm
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  ERROR_QUIET
 )
ENDFOREACH ()

IF (CMAKE_HOST_UNIX)
 FILE(GLOB QM_FILES
  RELATIVE ${CMAKE_CURRENT_BINARY_DIR}
  REGEX "^antiquacrm_[a-z]{2,3}\.qm$"
 )
 INSTALL (FILES ${QM_FILES}
  DESTINATION ${ANTIQUACRM_TRANSLATION_TARGET}
  COMPONENT data
 )
 INSTALL (FILES
  ${PROJECTNAME}_untranslated.ts
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECTNAME}
  COMPONENT data
 )
ENDIF (CMAKE_HOST_UNIX)
