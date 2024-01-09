#!/usr/bin/env bash

set +x

_mime_db_tool=`which update-mime-database 2>/dev/null`

test -x "${_mime_db_tool}" || {
  echo "Install update-mime-database from shared-mime-info"
  exit 1
}

test -d $HOME/.local/share/mime/packages || mkdir -pm 0750 $HOME/.local/share/mime/packages

install -vm 0640 text-qcss.xml $HOME/.local/share/mime/packages/

update-mime-database -Vn $HOME/.local/share/mime
