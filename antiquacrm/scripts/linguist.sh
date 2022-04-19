#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_srcpath="$(dirname "$(readlink -f $0)")"

. ${_srcpath}/scriptsconfig.bash

_ts="$(ls ${_PROJECT_SOURCE}/i18n/*_de.ts)"

${_QT5_HOME}/bin/linguist ${_ts}
