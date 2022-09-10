#!/usr/bin/env bash

_file="${1:-"dummy"}"

test -e ${_file} || {
  echo "Kein Datei angegeben!"
  echo "Verwende $0 <datei>"
  exit 1
}

json_pp -f json -t dumper -json_opt utf8,pretty < ${_file}

