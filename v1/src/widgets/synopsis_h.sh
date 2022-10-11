#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
##################################################################################
# This file is part of the HJCMS Project
#
# Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2022
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

cat > AntiquaWidgets.cmake <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// This file is part of the AntiquaCRM Project
//
// Copyright (C) HJCMS https://www.hjcms.de, (C) 2007-$(date +"%Y")
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; see the file LICENSE.md.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//

EOF

_headers="$(ls *.h | sort)";

for i in ${_headers} ; do
  _header="$(basename ${i})";
  _macro=$(grep '^#ifndef ANTIQUACRM_[A-Z_]\+_H$' ${_header})
  if test -n "${_macro}" ; then
  cat >> AntiquaWidgets.cmake <<EOF
${_macro}
#include "${_header}"
#endif

EOF
fi
done

cat > AntiquaInputEdit.cmake <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// This file is part of the AntiquaCRM Project
//
// Copyright (C) HJCMS https://www.hjcms.de, (C) 2007-$(date +"%Y")
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; see the file LICENSE.md.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//

EOF

for i in ${_headers} ; do
  _header="$(basename ${i})";
  _macro=$(grep '^#ifndef ANTIQUA_INPUT_[A-Z]\+_H$' ${_header})
  if test -n "${_macro}" ; then
  cat >> AntiquaInputEdit.cmake <<EOF
${_macro}
#include "${_header}"
#endif

EOF
fi
done
