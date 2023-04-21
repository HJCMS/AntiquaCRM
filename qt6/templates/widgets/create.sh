#!/usr/bin/env bash
#
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

set +x

_script_base=${HOME}/Developement/antiqua/qt6/templates/widgets
_classname=$1

if [ $(basename ${_script_base}) != "widgets" ] ; then
cat <<EOF
 Invalid target ...
EOF
  exit 1
fi

test -n "${_classname}" || {
cat <<EOF
 Missing a valid Classname.
 Set it now in CaseSensitive and press <enter>
EOF
  read -r _classname
}

test -n "${_classname}" || {
  echo "Empty input, abort ..."
  exit 1
}

_classname="$(echo ${_classname} | sed 's, ,_,g')"

_macro="`echo ${_classname} | awk '{print toupper($1)}'`"

_filename="`echo ${_classname} | awk '{print tolower($1)}'`"

if [[ -e ${_filename}.h ]] || [[ -e ${_filename}.cpp ]]  ; then
  echo "Already Exits - abort!"
  ls -ltGh ${_filename}.* 2> /dev/null
  exit 1
fi

if test -r ${_script_base}/input_tpl.h ; then
 echo "Generate: ${_classname}.h"
 cat ${_script_base}/input_tpl.h | \
   sed "s,@YEAR@,$(date +"%Y"),gm" | \
   sed "s,@CLASSMACRO@,${_macro},gm" | \
   sed "s,@CLASSNAME@,${_classname},m" > ${_filename}.h
fi

if test -r ${_script_base}/input_tpl.cpp ; then
 echo "Generate: ${_filename}.cpp"
 cat ${_script_base}/input_tpl.cpp | \
   sed "s,@CLASSNAME@,${_classname},gm" | \
   sed "s,@INCLUDE@,${_filename},m" > ${_filename}.cpp
fi

echo "created ..."
ls -ltGh ${_filename}.{h,cpp}
cat <<EOF

// AntiquaCRM::${_classname}
#include "${_filename}.h"

EOF

## EOF

