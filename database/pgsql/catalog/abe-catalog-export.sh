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

_sqlfile=$HOME/.config/HJCMS/abebooks-catalog-export.sql
_csv_header=$HOME/.config/HJCMS/abebooks-catalog-header.csv

## Ausgabe Verzeichnis
_output_target=${_dest}/catalog

# Standard Ausgabename für Temporäre Daten in ~/.cache erstellen!
_tempdir=$HOME/.cache
_output="catalog-${_date}-ISO-8859-15.csv"

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

## WARNING - Keine Leerzeichen verwenden!
_header=""

psql \
  --csv \
  --no-align \
  --echo-hidden \
  --tuples-only \
  --field-separator="`printf '\t'`" \
  --log-file=${_tempdir}/catalog-error.log \
  --output=${_tempdir}/catalog-export.temp < ${_sqlfile}

if test -s ${_tempdir}/catalog-export.temp ; then
  cat ${_csv_header} > ${_tempdir}/catalog-export-utf8.csv
  sed 's,NOT_SET,,g' ${_tempdir}/catalog-export.temp >> ${_tempdir}/catalog-export-utf8.csv
  touch ${_tempdir}/iconv.log
  iconv -f UTF8//IGNORE -t LATIN9 --output=${_tempdir}/${_output} ${_tempdir}/catalog-export-utf8.csv 2> ${_tempdir}/iconv.log
  if test $? != 0 ; then
    echo "ICONV ERRORS: ${_tempdir}/iconv.log"
    LC_ALL=de_DE@euro tail ${_tempdir}/${_output}
  fi
  ## cp ${_tempdir}/catalog-export-utf8.csv ${_tempdir}/${_output}
  mv -b ${_tempdir}/${_output} ${_output_target}/
  mv -b ${_tempdir}/catalog-export-utf8.csv ${_output_target}/
  _lines="$(wc -l ${_output_target}/${_output} 2>/dev/null | cut -d' ' -f1)"
  rm -f ${_output_target}/catalog.csv
  ln -sf ${_output} ${_output_target}/catalog.csv
  rm -f ${_tempdir}/catalog-export.temp
  echo ""
  echo "Katalog Ertellt am '`date`' mit '`echo "${_lines} - 1" | bc`' einträgen."
  ls -lh "${_output_target}/catalog-export-utf8.csv"
fi

exit $?

##
## EOF
##
