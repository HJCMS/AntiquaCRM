#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
##################################################################################
# This file is part of the HJCMS Project
#
# Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2022
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
##################################################################################

set +x

_script_base=$(dirname $0)
_classname=$1

if [ $(basename ${_script_base}) != "templates" ] ; then
  echo "Invalid target ..."
  echo "Usage ./templates/`basename $0` <CLASSNAME>"
  exit 1
fi

test -n "${_classname}" || {
  echo -n "Classname not set: "
  read -r _classname
}

test -n "${_classname}" || {
  echo "abort ..."
  exit 1
}

_class="$(echo ${_classname} | sed 's, ,_,g')"

_proc_h="`echo ${_class} | awk '{print toupper($1)}'`"

_filename="`echo ${_class} | awk '{print tolower($1)}'`"

if [[ -e ${_filename}.h ]] || [[ -e ${_filename}.cpp ]]  ; then
  echo "Already Exits - abort!"
  ls -ltGh ${_filename}.* 2> /dev/null
  exit 1
fi

cat ./templates/input.h.tpl | \
  sed "s,@CPPNAME@,${_proc_h},m" | \
  sed "s,@CLASSNAME@,${_class},m" > ${_filename}.h

cat ./templates/input.cpp.tpl | \
  sed "s,@CLASSNAME@,${_class},gm" | \
  sed "s,@INCLUDE@,${_filename},m" > ${_filename}.cpp

echo "created ..."
ls -ltGh ${_filename}.{h,cpp}
echo
