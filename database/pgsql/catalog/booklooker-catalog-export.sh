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

test -e $HOME/.config/postgres/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

. $HOME/.config/postgres/servercfg.bash

_sqlfile=${_scripting}/catalog/booklooker-export.sql
_header=${_scripting}/catalog/booklooker-export.header

## Ausgabe Verzeichnis
_output_target=${_dest}/catalog

# Standard Ausgabename für Temporäre Daten in ~/.cache erstellen!
_tempdir=$HOME/.cache
_output="booklooker-${_date}.csv"

#####################################################################

if test ! -e ${_sqlfile} ; then
  echo "SQL File not found!"
  echo "- ${_sqlfile} "
  exit 1
fi

if test ! -d ${_output_target} ; then
  echo "Invalid output target!"
  exit 1
fi

__init_tempfile

__init_sql_enviroment

cd ${_output_target}

psql \
  --csv \
  --no-align \
  --echo-hidden \
  --tuples-only \
  --field-separator="`printf '\t'`" \
  --log-file=${_tempdir}/catalog-error.trace \
  --output=${_tempdir}/catalog-export.temp < ${_sqlfile}

if test -s ${_tempdir}/catalog-export.temp ; then
  cat ${_header} | head -n 1 | sed 's/|/\t/g' > ${_tempdir}/catalog-export-utf8.csv
  sed 's,NOT_SET,,g' ${_tempdir}/catalog-export.temp >> ${_tempdir}/catalog-export-utf8.csv
  mv -b ${_tempdir}/catalog-export-utf8.csv ${_output_target}/${_output}
  _lines="$(wc -l ${_output_target}/${_output} 2>/dev/null | cut -d' ' -f1)"
  echo "Katalog: '`date`' mit '`echo "${_lines} - 1" | bc`' einträgen."
  rm -f ${_tempdir}/catalog-export.temp
fi

exit $?

##
## EOF
##
