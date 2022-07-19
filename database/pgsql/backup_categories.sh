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

set +x

_sourcepath="$(dirname "$(readlink -f $0)")"

test -e ${_sourcepath}/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

source ${_sourcepath}/servercfg.bash

echo "-- pgdump > categories_extern.backup.sql"
pg_dump \
  --host=${_server} \
  --port=${_config[1]} \
  --dbname=${_config[2]} \
  --username=${_config[3]} \
  --table=categories_extern \
  --no-password \
  --no-owner \
  --if-exists \
  --clean \
  --inserts  \
  --attribute-inserts \
  --file=categories_extern.backup.sql

cat >> categories_extern.backup.sql <<EOF

SELECT pg_catalog.setval('public.categories_extern_id_seq', (SELECT MAX(ce_id) FROM categories_extern), true);

--
-- DONE
--
EOF

cat > $HOME/.cache/categories_extern_query.sql <<EOF
SELECT s.ce_id,
  m.ce_name AS main_category,
  s.ce_name AS sub_category,
  s.ce_company_keywords AS keywords
FROM categories_extern AS m
LEFT JOIN categories_extern AS s ON s.ce_binding=m.ce_id
WHERE length(s.ce_company_keywords)>0 ORDER BY main_category;

EOF

PGSERVICE=$USER psql < $HOME/.cache/categories_extern_query.sql
