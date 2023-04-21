# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
###############################################################

# Add supported translation language here ...
SET(supported_languages
 de
)

# BEGIN::BUILD
EXECUTE_PROCESS (COMMAND ${QT_QMAKE_EXECUTABLE}
 -query QT_INSTALL_BINS OUTPUT_VARIABLE QT_BINARY_TARGET)

FIND_PROGRAM (LRELEASE_EXECUTABLE
 "lrelease" "lrelease-qt6" "lrelease-libqt6" "qt6-lrelease" "libqt6-lrelease"
 PATHS ${QT_BINARY_TARGET})

SET(LRELEASE_OPTIONS -silent -compress -nounfinished -removeidentical)

FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
FOREACH (TR_LANGUAGE ${supported_languages})
 MESSAGE (STATUS "Generate antiquacrm_${TR_LANGUAGE}.qm translation.")
 EXECUTE_PROCESS (COMMAND ${LRELEASE_EXECUTABLE} ${LRELEASE_OPTIONS}
    ${CMAKE_CURRENT_SOURCE_DIR}/antiquacore_${TR_LANGUAGE}.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm_${TR_LANGUAGE}.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/qt6-tr/translations/qtbase_${TR_LANGUAGE}.ts
    -qm ${CMAKE_CURRENT_BINARY_DIR}/antiquacrm_${TR_LANGUAGE}.qm
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  ERROR_QUIET
 )
ENDFOREACH ()
# END::BUILD

# BEGIN::INSTALL
IF (CMAKE_HOST_UNIX)
 FOREACH (TR_LANGUAGE ${supported_languages})
 MESSAGE (STATUS "Install antiquacrm_${TR_LANGUAGE}.qm")
 INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/antiquacrm_${TR_LANGUAGE}.qm
   DESTINATION ${ANTIQUACRM_TRANSLATION_TARGET} COMPONENT data)
 ENDFOREACH ()
 INSTALL (FILES
   antiquacrm_untranslated.ts
   DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECTNAME}
   COMPONENT data
 )
ENDIF (CMAKE_HOST_UNIX)
# END::INSTALL
