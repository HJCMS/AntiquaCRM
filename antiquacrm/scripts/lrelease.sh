#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_srcpath="$(dirname "$(readlink -f $0)")"

. ${_srcpath}/scriptsconfig.bash

_build_dest="${_PROJECT_TARGET}/build-antiquacrm-Desktop_Qt_5_15_2_GCC_64bit-Debug/src/i18n"
# _ts_qt5=""
_ts_project="${_PROJECT_SOURCE}/i18n/antiquacrm_de.ts"

mkdir -p ${_build_dest}
rm -f ${_build_dest}/antiquacrm_de.qm
${_QT5_HOME}/bin/lrelease -removeidentical ${_ts_project} -qm ${_build_dest}/antiquacrm_de.qm
