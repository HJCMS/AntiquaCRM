#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

if test -d ~/Developement/antiqua ; then
  rm -rf ~/Developement/antiqua/build-antiquacrm-*
  find ~/Developement/antiqua -type f -name "*~" -exec rm -v "{}" \;
fi

function __check_sub_project()
{
  local _f=${1:-"."}/CMakeLists.txt
  if test -s ${_f} ; then
    grep -e '_subproject\b\s\+\"' ${_f} | cut -d'"' -f2
  fi
}

echo "# CMAKE Targets" > CMakeSubTargets.cmake

cat > CMakeSubTargets.cmake <<EOF
##
# Generated by $0
##
SET (TARGET_INCLUDE_DIRS
 \${PROJECT_INCLUDE_DIRS}
 \${CMAKE_CURRENT_BINARY_DIR}/src
 \${CMAKE_CURRENT_SOURCE_DIR}/src
EOF

declare -a _buffer=();

for _t in $(find . -mindepth 1 -maxdepth 1 -type d -exec basename "{}" \;) ; do
if test -e ${_t}/CMakeLists.txt ; then
  _buffer+=(${_t})
  cat >> CMakeSubTargets.cmake <<EOF
 \${CMAKE_CURRENT_BINARY_DIR}/${_t}
 \${CMAKE_CURRENT_SOURCE_DIR}/${_t}
EOF
fi
done

cat >> CMakeSubTargets.cmake <<EOF
 \${CMAKE_CURRENT_BINARY_DIR}/src
 \${CMAKE_CURRENT_SOURCE_DIR}/src
)

INCLUDE_DIRECTORIES (\${TARGET_INCLUDE_DIRS})

EOF

for _s in ${_buffer[@]} ; do
cat >> CMakeSubTargets.cmake <<EOF
ADD_SUBDIRECTORY (${_s})
EOF
done

cat >> CMakeSubTargets.cmake <<EOF

SET(_link_sub_libs
EOF

for _s in ${_buffer[@]} ; do
  __check_sub_project ${_s} >> CMakeSubTargets.cmake
done

cat >> CMakeSubTargets.cmake <<EOF
\${QM_FILES}
)
EOF
