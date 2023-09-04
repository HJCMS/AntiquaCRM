#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_func="${1:-"help"}"
_build_dir="build-qt5-debug"
_lang="de"
_dirs="`find . -maxdepth 1 -type d -not -name plugins -not -name qt5-tr -exec basename "{}" \;`"

case "${_func}" in
  *update)
    lupdate-qt5 \
      -no-obsolete \
      -locations relative \
      -recursive ${_dirs} \
      -ts $(ls ./i18n/antiqua*.ts)
  ;;

  *release)
    lrelease-qt5 \
      -compress \
      -verbose \
      -nounfinished \
      -removeidentical \
      $(find ./i18n -mindepth 1 -maxdepth 1 -name "*.ts" -not -name antiquacrm_untranslated.ts) \
      -qm ../../${_build_dir}/src/i18n/antiquacrm_${_lang}.qm
  ;;

  edit|linguist)
    if test -e ./i18n/antiquacrm_de.ts ; then
      linguist-qt5 ./i18n/antiquacrm_${_lang}.ts
    fi
  ;;

  help)
    echo -e "\tUsage: `basename $0` {update|release|edit}"
    exit 0
  ;;

esac

exit $?
