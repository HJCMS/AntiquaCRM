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

_sourcepath="$(dirname "$(readlink -f $0)")"
_schema="public"

test -e ${_sourcepath}/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

chmod -c 0600 ${_sourcepath}/servercfg.bash

source ${_sourcepath}/servercfg.bash

test -d ./import || {
  echo "Missing ./import Target"
  exit 1
}

test -s ./psql-migration-order.txt || {
  echo "File 'psql-migration-order.txt' not exists or empty?"
  exit 1
}

echo "Show Disk usage ..."
du -h ./import/

echo "Begin IMPORT's from ..." > psql-migration.log
while read _sql ; do
  _file="./import/${_sql}.sql"
  if test -s ${_file} ; then
    echo "-- Import ${_file} --"
    PGSERVICE=$USER psql < ${_file} 1>> psql-migration.log
  fi
done < ./psql-migration-order.txt

## EOF
