#!/usr/bin/env bash
##############################################
## Arbeitsverzeichnis
_scripting="$HOME/Developement/antiqua/database/pgsql"
_dest="$HOME/Developement/antiqua/database/pgsql/Database"
## (Jahr/Tag von Jahr)
_date="$(date +"%Y%j")"
## Export aus Schema
_scheme="public"
## Benutzer
_user=$USER
## Remote Server
_server="pvserver.hjcms"
## Standard Ausgabename für Temporäre Daten
_output="db_${_scheme}_export"

## @see export-all-tables.sh
_sql_query_tables="SELECT CONCAT('${_scheme}.',tablename) AS t FROM pg_catalog.pg_tables WHERE schemaname != 'pg_catalog' AND  schemaname != 'information_schema';"

# Alle SQL-DUMPS in dieses Zielverzeichnis schreiben!
function __create_targets()
{
  test -d ${_dest}/${_date} || {
    mkdir -p ${_dest}/${_date}
  }

  test -d ${_dest}/${_date} || {
    echo "mkdir permission denied."
    exit 1
  }

  _link="`basename ${_dest}`/${_date}"
  if test ! -L ./current ; then
    ln -sf ${_link} ./current
  fi
  
  if [[ "$(readlink ./current)" != "${_link}" ]] ; then
    rm -f ./current
    ln -sf ${_link} ./current
  fi
}

# Lese in Array Enviroment _config
declare -a _config=();

# Lese .pgpass und .pg.service.conf, setzte die Umgebungs-Variablen.
function __init_sql_enviroment()
{
  test -e $HOME/.pgpass || {
    echo "FATAL: Missing ~/.pgpass"
    exit 1
  }

  _ifs=$IFS
  IFS=':'
  ## {hostname|socket}:port:database:username:password
  _config=($(cat $HOME/.pgpass | grep -v '^#' | grep "${_server}"))

  IFS=${_ifs}

  grep ${_server} ~/.pg_service.conf 1>&2>/dev/null
  if test $? = 0 ; then
    export PGSERVICE=${_user}
  fi
}

# Temporäre Daten in ~/.cache erstellen!
function __init_tempfile()
{
  test -d $HOME/.cache || {
    mkdir -m 0750 $HOME/.cache
  }
  rm -f $HOME/.cache/${_output}.sql
  touch $HOME/.cache/${_output}.sql
}

##EOF
