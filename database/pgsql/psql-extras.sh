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
_schema="public"

test -e ${_sourcepath}/servercfg.bash || {
  echo "FATAL: Can not read Configuration"
  exit 1
}

chmod -c 0600 ${_sourcepath}/servercfg.bash

source ${_sourcepath}/servercfg.bash

test -d ${_scripting}/export || {
  echo "Missing ./export Target"
  exit 1
}

cat > $HOME/.cache/update-mariadb-isbn.sql <<EOF
-- UPDATE ISBN
UPDATE xgr SET info=REPLACE(REPLACE(REPLACE(REPLACE(info,'-',''),'x',''),'X',''),'"','') WHERE info LIKE 'ISBN%';
UPDATE xgr SET info=REPLACE(info,' ','') WHERE info LIKE 'ISBN%';
UPDATE xgr SET info=REPLACE(info,' Auflage','') WHERE info LIKE '%Auflage%';
UPDATE xgr SET info=REPLACE(info,'Tsd','') WHERE info LIKE '%Tsd%';
UPDATE xgr SET info=TRIM(REPLACE(info,'.','')) WHERE info LIKE '%.%';
UPDATE xgr SET info=REPLACE(info,'ISBN','EAN:') WHERE info LIKE 'ISBN%';
UPDATE xgr SET info=TRIM(info) WHERE info LIKE 'EAN:%' AND length(info) >= 14;

-- SELECT info FROM xgr WHERE info LIKE 'EAN:%' AND length(info) = 14;

SELECT CONCAT("UPDATE public.inventory_books SET ib_isbn=",REPLACE(info,'EAN:','')," WHERE ib_id=",artnr," AND ib_isbn=0;") AS "-- Update ISBN"
FROM xgr WHERE (info LIKE 'EAN:%' AND length(info) = 14);

EOF

mysql < $HOME/.cache/update-mariadb-isbn.sql > ${_scripting}/export/public.inventory_books.UPDATE.ib_isbn_2.sql

rm -vf $HOME/.cache/update-mariadb-isbn.sql

cat > $HOME/.cache/update-mariadb-keywords.sql <<EOF
UPDATE xgrkategorie_zvab SET kategorie_bez='Autobiographien' WHERE kategorie_key=1600;
UPDATE xgrkategorie_zvab SET kategorie_bez='Bildbände & Naturführer' WHERE kategorie_key=3510;
UPDATE xgrkategorie_zvab SET kategorie_bez='Erzählungen & Gedichte' WHERE kategorie_key=1500;
UPDATE xgrkategorie_zvab SET kategorie_bez='Geografie' WHERE kategorie_key=6630;
UPDATE xgrkategorie_zvab SET kategorie_bez='Geschichte' WHERE kategorie_key=5599;
UPDATE xgrkategorie_zvab SET kategorie_bez='Kulturgeschichte' WHERE kategorie_key=5510;
UPDATE xgrkategorie_zvab SET kategorie_bez='Mundart' WHERE kategorie_key=5630;
UPDATE xgrkategorie_zvab SET kategorie_bez='Ortsgeschichte' WHERE kategorie_key=3210;
UPDATE xgrkategorie_zvab SET kategorie_bez='Reisebeschreibungen' WHERE kategorie_key=3610;
UPDATE xgrkategorie_zvab SET kategorie_bez='Romane' WHERE kategorie_key=1100;
UPDATE xgrkategorie_zvab SET kategorie_bez='Sagen & Märchen' WHERE kategorie_key=1400;
UPDATE xgrkategorie_zvab SET kategorie_bez='Bilderbücher' WHERE kategorie_key=2100;
UPDATE xgrkategorie_zvab SET kategorie_bez='Sachbücher' WHERE kategorie_key=2899;

DELETE FROM xgrkategorie_zvab WHERE kategorie_bez='Deutschland' OR kategorie_bez='Sonstiges' OR kategorie_bez='Afrika';

SELECT CONCAT("UPDATE public.inventory_books SET ib_keyword='",zvab.kategorie_bez,"' WHERE (ib_id=",xgr.artnr," AND ib_keyword IS NULL);") AS "-- Update Keywords"
FROM xgr
LEFT JOIN xgrkategorie_zvab AS zvab ON zvab.kategorie_key=kategorie_zvab
WHERE xgr.art LIKE 'bu' AND xgr.artnr>0 AND length(xgr.titel)>1 AND zvab.kategorie_bez IS NOT NULL
ORDER BY xgr.artnr ASC;
EOF

mysql < $HOME/.cache/update-mariadb-keywords.sql > ${_scripting}/export/public.inventory_books.UPDATE.ib_keywords.sql

rm -vf $HOME/.cache/update-mariadb-keywords.sql
