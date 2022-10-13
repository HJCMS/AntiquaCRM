#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_build_type="build-v1-Desktop_Qt_5_15_2_GCC_64bit-Debug"

function lupdate() {
  lupdate-qt5 \
    -no-obsolete \
    -locations relative \
    -recursive . \
    -ts $(ls ./i18n/antiqua*.ts)
}

function lrelease() {
  lrelease-qt5 \
    -compress \
    -verbose \
    -nounfinished \
    -removeidentical \
    $(find ./i18n -name "*.ts" -not -name antiquacrm_untranslated.ts) \
    -qm ../../${_build_type}/src/i18n/
}

lupdate
