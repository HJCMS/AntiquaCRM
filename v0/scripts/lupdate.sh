#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_srcpath="$(dirname "$(readlink -f $0)")"

. ${_srcpath}/scriptsconfig.bash

_ts_files="$(ls ${_PROJECT_SOURCE}/i18n/antiqua*.ts)"

cd ${_PROJECT_SOURCE}
${_QT5_HOME}/bin/lupdate -no-obsolete -locations relative -recursive . -ts ${_ts_files}
