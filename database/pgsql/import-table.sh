#!/usr/bin/env bash

test -n "$1" || {
cat <<EOF

  Verwende: $0 tabellename
  Optional: $0 tabellename verzeichnis
  Beispiel: $0 "inventory_books" "2022-03-21"

EOF
  exit 1
}

_sourcepath="$(dirname "$(readlink -f $0)")"

test -e ${_sourcepath}/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

chmod -c 0600 ${_sourcepath}/servercfg.bash

source ${_sourcepath}/servercfg.bash

_table="$1"
_date="${2:-"${_date}"}"

test -e ${_dest}/${_date}/${_output}_${_table}.sql || {
  echo "No SQL Import-File exists !!!"
  exit 1
}

declare -a _config=();

function __getconfig()
{
  IFS=':'
  ## hostname:port:database:username:password
  _config=($(cat $HOME/.pgpass | grep -v '^#' | grep "$USER"))
}

if test -e $HOME/.pgpass ; then
  __getconfig
else
  echo "FATAL: Missing ~/.pgpass"
  exit 1
fi

grep ${_server} ~/.pg_service.conf 1>&2>/dev/null

if test $? = 0 ; then
  export PGSERVICE=${_user}
fi

echo "-- IMPORT: ${_date}/${_output}_${_table}.sql"
psql -h ${_config[0]} -d ${_config[2]} -U ${_config[3]} < ${_dest}/${_date}/${_output}_${_table}.sql

##EOF
