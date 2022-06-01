#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_srcpath="$(dirname "$(readlink -f $0)")"

. ${_srcpath}/scriptsconfig.bash

_BUILDDIR=$HOME/.cache/antiquacrm
if test -d ${_BUILDDIR} ; then
  rm -rf ${_BUILDDIR}
fi

mkdir -m 0755 -p ${_BUILDDIR}

_QT5_NINJA_SETUP="${_QT5_CREATOR_HOME}/share/qtcreator/package-manager/auto-setup.cmake"

export PATH="${_QT5_HOME}/bin:${_QT5_TOOLS_HOME}/Ninja:$PATH"

pushd ${_BUILDDIR}

  ln -s ${_QT5_TOOLS_HOME}/Ninja/ninja ninja

  cmake -Wno-dev -Wno-deprecated \
    -DCMAKE_GENERATOR:STRING=Ninja \
    -DCMAKE_BUILD_TYPE:STRING=MINSIZEREL \
    -DCMAKE_PREFIX_PATH:PATH=${_BUILDDIR}/usr \
    -DANTIQUA_LSB_INSTALL:BOOL=OFF \
    -DCMAKE_INSTALL_PREFIX:PATH=${_BUILDDIR}/opt/antiquacrm \
    -DCMAKE_PROJECT_INCLUDE_BEFORE:FILEPATH=${_QT5_NINJA_SETUP} \
    -DQT_QMAKE_EXECUTABLE:FILEPATH=${_QT5_HOME}/bin/qmake \
    -DLIB_SUFFIX:STRING=64 \
    -DCMAKE_SKIP_RPATH:BOOL=ON \
    ${_PROJECT_TARGET}/antiquacrm

  echo "./ninja build"
  ./ninja

  echo "./ninja install"
  ./ninja install

popd
