#!/usr/bin/env bash

set +x

_qthome=$HOME/Programme/Qt/5.15.2/gcc_64
_lrelease=${_qthome}/bin/lrelease
_out="../../build-antiquacrm-Desktop_Qt_5_15_2_GCC_64bit-Debug/src/i18n"
_ts_files="$(ls $PWD/i18n/antiquacrm_de.ts)"

mkdir -p ${_out}
pushd ${_out}
  rm -f antiquacrm_de.qm
  ${_lrelease} -removeidentical ${_ts_files} -qm antiquacrm_de.qm
popd
