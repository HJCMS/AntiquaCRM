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

_sqlfile=$HOME/.config/HJCMS/booklooker-catalog-export.sql
_csv_header=$HOME/.config/HJCMS/booklooker-catalog-export.txt
_bl_config=$HOME/.config/HJCMS/booklooker-remote.cfg
_token=""

## Ausgabe Verzeichnis
_output_target=${_dest}/catalogs

# Standard Ausgabename für Temporäre Daten in ~/.cache erstellen!
_tempdir=$HOME/.cache
_logdir=$HOME/.cache
_output="booklooker-${_date}.csv"

#####################################################################

if test ! -e ${_sqlfile} ; then
  echo "PgSQL file not found!"
  echo "- ${_sqlfile} "
  echo "aborted"
  exit 1
fi

if test ! -d ${_output_target} ; then
  echo "Catalog output target not exists!"
  echo "aborted"
  exit 1
fi

if test ! -s ${_bl_config} ; then
  echo "Remote access Configuration mismatch!"
  echo "aborted"
  exit 1
fi

. ${_bl_config}

function __authentication() {
  test -n "${_bl_api_host}" || {
    echo "\${_bl_api_host} is empty"
    exit 1
  }

  test -n "${_bl_api_key}" || {
    echo "\${_bl_api_key} is empty"
    exit 1
  }

  curl \
    --ssl-reqd \
    --tlsv1.2 \
    -X POST ${_bl_api_host}/2.0/authenticate \
    -H "Accept: text/*;application/json,q=0.1" \
    -H "Cache-Control: no-cache, private" \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -d "apiKey=${_bl_api_key}" \
    --trace ${_logdir}/booklooker.log \
    --output ${_tempdir}/booklooker.json

  if test $? -ne 0 ; then
    echo "curl finished with errors!"
    exit 1
  fi

  if test -s ${_tempdir}/booklooker.json ; then
    local _buffer=$(cat ${_tempdir}/booklooker.json | sed 's,[\"\{\} ],,g')
    if test -n "$(echo ${_buffer} | cut -d, -f1 | grep "status:OK")" ; then
      echo ${_buffer} | cut -d, -f2 | cut -d: -f2
    else
      echo "ERROR: $(cat ${_tempdir}/booklooker.json)"
      exit 1
    fi
  else
    echo "no token"
    exit 1
  fi
}

function __post_catalog() {
  local _upload_file=${1:-"dummy"}
  test -s "${_upload_file}" || {
    echo "\${_upload_file} is empty"
    exit 1
  }

  test -n "${_bl_api_host}" || {
    echo "\${_bl_api_host} is empty"
    exit 1
  }

  test -n "${_bl_api_key}" || {
    echo "\${_bl_api_key} is empty"
    exit 1
  }

  test -n "${_bl_api_key}" || {
    echo "\${_bl_api_key} is empty"
    exit 1
  }

  curl \
    --ssl-reqd \
    --tlsv1.2 \
    -X POST "${_bl_api_host}/2.0/file_import?token=$_token" \
    -H "Accept: text/*;application/json,q=0.1" \
    -H "Cache-Control: no-cache, private" \
    -H "Content-Type: multipart/form-data; boundary='upload'" \
    -F "fileType=article" \
    -F "dataType=1" \
    -F "mediaType=0" \
    -F "formatID=booklooker" \
    --trace ${_logdir}/booklooker.log \
    --output ${_tempdir}/booklooker.json
    --upload-file ${_upload_file}

  if test $? -ne 0 ; then
    echo "Curl finished with errors!"
    exit 1
  fi
}

__init_sql_enviroment

cd ${_output_target}

psql \
  --csv \
  --no-align \
  --echo-hidden \
  --tuples-only \
  --field-separator="`printf '\t'`" \
  --log-file=${_logdir}/catalog-error.trace \
  --output=${_tempdir}/catalog-export.temp < ${_sqlfile}

if test -s ${_tempdir}/catalog-export.temp ; then
  cat ${_csv_header} | head -n 1 | sed 's/|/\t/g' > ${_logdir}/catalog-export-utf8.csv
  sed 's,NOT_SET,,g' ${_tempdir}/catalog-export.temp >> ${_logdir}/catalog-export-utf8.csv
  mv -b ${_logdir}/catalog-export-utf8.csv ${_output_target}/${_output}
  _lines="$(wc -l ${_output_target}/${_output} 2>/dev/null | cut -d' ' -f1)"
  echo "Catalog: '`date`' with '`echo "${_lines} - 1" | bc`' lines created."
  rm -f ${_tempdir}/catalog-export.temp
fi

# Ihre Angebotsdatei können Sie entweder als Textdatei oder als komprimiertes ZIP-Archiv übergeben.
# Standardmäßig müssen die Textdateien in der Kodierung "ISO 8859-1" vorliegen.
# Wenn Sie eine andere Kodierung, bspw. UTF-8 verwenden,
# wenden Sie sich bitte per E-Mail an daten@booklooker.de.
if test -s ${_output_target}/${_output} ; then
  _token="$(__authentication)"
  sleep 1
  if test -n "${_token}" ; then
    __post_catalog ${_output_target}/${_output}
  fi
fi

exit $?

##
## EOF
##
