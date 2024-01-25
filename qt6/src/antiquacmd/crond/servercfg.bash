#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
##################################################################################
# This file is part of the HJCMS Project
#
# Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2024
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
# NOTE All cron jobs running in a Standard User Environment!

##
# Working directory
#
_scripting="$HOME/Downloads"

##
# Target directory
#
_dest="$HOME/Downloads"

##
# (year/day of year)
#
_date="$(date +"%Y%j")"

##
# export from database schema
#
_scheme="public"

##
# database user
# https://www.postgresql.org/docs/current/libpq-pgpass.html
#
_user=$USER

##
# remote server
# The remote server to use from .pgpass file.
#
_server="example.com"

##
# Default output name for temporary data
#
_output="db_${_scheme}_export"

##
# Required by Export Catalog
#
_catalog_sqlfile=$HOME/.config/HJCMS/default-catalog-export.sql
_catalog_header=$HOME/.config/HJCMS/default-catalog-export.header

##
# @see Backup: export-all-tables.sh
#
_sql_query_tables="SELECT CONCAT('${_scheme}.',tablename) AS t FROM pg_catalog.pg_tables WHERE schemaname != 'pg_catalog' AND  schemaname != 'information_schema';"

_sql_query_views="SELECT CONCAT('${_scheme}.',viewname) AS t FROM pg_catalog.pg_views WHERE schemaname != 'pg_catalog' AND  schemaname != 'information_schema';"

##
# All SQL-DUMPS written to this target directory!
#
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
    ln -sf ../${_link} ./current
  fi

  if [[ "$(readlink ./current)" != "${_link}" ]] ; then
    rm -f ./current
    ln -sf ../${_link} ./current
  fi
}

##
# Create config Environment Array
#
declare -a _config=();

##
# Read .pgpass and .pg_service.conf and set environment variables.
#
# https://www.postgresql.org/docs/current/libpq-pgpass.html
# https://www.postgresql.org/docs/current/libpq-pgservice.html
#
function __init_sql_enviroment()
{
  test -e $HOME/.pgpass || {
    echo "FATAL: Missing ~/.pgpass"
    exit 1
  }

  test -e $HOME/.pg_service.conf || {
    echo "FATAL: Missing ~/.pg_service.conf"
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

##
# Create temporary data in ~/.cache/ target!
#
function __init_tempfile()
{
  test -d $HOME/.cache || {
    mkdir -m 0750 $HOME/.cache
  }
  rm -f $HOME/.cache/${_output}.sql
  touch $HOME/.cache/${_output}.sql
}

##EOF
