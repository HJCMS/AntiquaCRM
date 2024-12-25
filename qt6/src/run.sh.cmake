#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
##################################################################################
# This file is part of the HJCMS Project
#
# Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2025
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

set +x

test -d ${CMAKE_CURRENT_BINARY_DIR} || exit 1

cd ${CMAKE_CURRENT_BINARY_DIR}

function _libdirs() {
  find ./Antiqua/ -mindepth 1 -type l \
     -name "lib*.so" -exec dirname "{}" \; | tr '\012' ':'
}

export PATH=${CMAKE_PREFIX_PATH}/bin:./plugins:$PATH

if test -n "$LD_LIBRARY_PATH" ; then
  export LD_LIBRARY_PATH=$(_libdirs).:$LD_LIBRARY_PATH
else
  export LD_LIBRARY_PATH=$(_libdirs).
fi

exec ./${PROJECTNAME} $@

##EOF
