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

test -e ${_sourcepath}/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

chmod -c 0600 ${_sourcepath}/servercfg.bash

source ${_sourcepath}/servercfg.bash

__create_targets

__init_sql_enviroment

__init_tempfile

function __export_table()
{
  _t=${1}

  rm -f $HOME/.cache/${_output}.sql

  echo "-- pgdump full Table backup: ${_t}.sql"
  pg_dump \
    --host=${_server} \
    --port=${_config[1]} \
    --dbname=${_config[2]} \
    --username=${_config[3]} \
    --table=${_t} \
    --no-password \
    --no-owner \
    --if-exists \
    --clean \
    --inserts  \
    --attribute-inserts \
    --file=$HOME/.cache/${_output}.sql

  mv -b $HOME/.cache/${_output}.sql ${_dest}/${_date}/${_t}.sql
}

test -n "$PGSERVICE" || {
  echo "Enviroment \$PGSERVICE not exits"
  exit 1
}

PGSERVICE=$PGSERVICE psql --csv -c "${_sql_query_tables}" > .tmp.log

_pattern="^${_scheme}"
while read _t; do
  if [[ "${_t}" =~ $_pattern ]] ; then
    __export_table ${_t}
  fi
done < .tmp.log

rm -f .tmp.log

echo "-- pgdump Structure Backup: schema-only.sql"
pg_dump \
  --host=${_config[0]} \
  --port=${_config[1]} \
  --dbname=${_config[2]} \
  --username=${_config[3]} \
  --encoding=UTF8 \
  --no-owner \
  --schema-only \
  --schema=${_scheme} \
  --create \
  --if-exists \
  --clean \
  --file=$HOME/.cache/schema-only.sql

mv -b $HOME/.cache/schema-only.sql ${_dest}/${_date}/schema-only.sql

echo "-- pgdump Structure Backup: Antiqua-PgSQL14.sql"
pg_dump \
  --host=${_config[0]} \
  --port=${_config[1]} \
  --dbname=${_config[2]} \
  --username=${_config[3]} \
  --encoding=UTF8 \
  --no-owner \
  --no-comments \
  --schema-only \
  --schema=${_scheme} \
  --clean \
  --create \
  --if-exists \
  --strict-names \
  --no-owner \
  --file=$HOME/.cache/Antiqua-PgSQL14.sql

perl -pi -e 's,\bheinemann\b,antiqua,g' $HOME/.cache/Antiqua-PgSQL14.sql

##EOF
