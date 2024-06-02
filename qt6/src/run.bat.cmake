rem Create Env
SET PATH=${QT_BUILD_PATH};.\lib;.\plugins;%PATH%
rem Start application
${CMAKE_CURRENT_BINARY_DIR}/${PROJECTNAME}.exe
