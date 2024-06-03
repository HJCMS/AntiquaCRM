#!/usr/bin/env bash

function _childs() {
  declare -a _list;
  for i in `find ./Antiqua/ -mindepth 1 -type l -name "*.so" -exec dirname "{}" \;` ; do
    if test -n "$_list" ; then
      _list="$_list:$i";
    else
      _list="$i";
    fi
  done
  echo $_list
}

export PATH=${CMAKE_PREFIX_PATH}/bin:$(_childs):./plugins:$PATH
${CMAKE_CURRENT_BINARY_DIR}/${PROJECTNAME}
