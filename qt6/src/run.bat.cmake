rem Create Env
SET PATH=${CMAKE_PREFIX_PATH}\bin;.\lib;.\plugins;%PATH%
rem Start application
${CMAKE_CURRENT_BINARY_DIR}/${PROJECTNAME}.exe
