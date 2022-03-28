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

test -n "$1" || {
  echo "Usage: `basename $0` table_name"
  exit 1
}
_t="$1"

_sourcepath="$(dirname "$(readlink -f $0)")"

test -e ${_sourcepath}/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

chmod -c 0600 ${_sourcepath}/servercfg.bash

source ${_sourcepath}/servercfg.bash

__create_targets

__init_sql_enviroment

__init_tempfile

pg_dump \
  -h ${_server} \
  -d ${_config[2]} \
  -U ${_config[3]} \
  -w -O -s -n "${_scheme}" \
  -t "${_t}" | tee $HOME/.cache/${_output}.sql 1> /dev/null

mv -b $HOME/.cache/${_output}.sql ${_dest}/${_date}/${_scheme}.${_t}.sql

##EOF
