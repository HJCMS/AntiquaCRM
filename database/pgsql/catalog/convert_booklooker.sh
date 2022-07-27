#!/usr/bin/env bash

_table="public.ref_booklooker";

cat > ${_table}.sql <<EOF
-- Booklooker Referenz Tabelle für Spartenzuweisung
-- Wird von "categories_extern" verwendet um Stichwörter zu zuweisen!
--
-- ALTER TABLE public.categories_intern ADD COLUMN ci_booklooker_id smallint DEFAULT 0;
--

ALTER TABLE IF EXISTS ONLY public.categories_intern DROP CONSTRAINT IF EXISTS booklooker_id;

DROP TABLE IF EXISTS ${_table};

CREATE TABLE ${_table} (
  bl_id smallint DEFAULT 0,
  bl_section character varying(80) NOT NULL
);

COMMENT ON TABLE ${_table} IS 'Booklooker Sparten Definitionen';
COMMENT ON COLUMN ${_table}.bl_id IS 'Sparten-Nr.';
COMMENT ON COLUMN ${_table}.bl_section IS 'Spartenname';

ALTER TABLE ONLY ${_table} ADD CONSTRAINT booklooker_id UNIQUE (bl_id);
ALTER TABLE ONLY ${_table} ADD CONSTRAINT uniq_bl_section UNIQUE (bl_section);

EOF

> tmp.sql

IFS="|"
while read line ; do
  arr=($line);
  if [[ ${arr[1]} =~ .*(/|-|,| ).* ]] ; then
    continue
  fi
  if test -n "${arr[2]}" ; then
    number=$(printf "%05d" ${arr[2]})
    echo "INSERT INTO ${_table} (bl_id,bl_section) VALUES ($number,'${arr[1]}');" >> tmp.sql
  fi
done < booklooker_sparten.csv

cat tmp.sql | sort | uniq >> ${_table}.sql

cat >> ${_table}.sql <<EOF

-- ALTER TABLE ONLY public.categories_intern ADD CONSTRAINT booklooker_id
--  FOREIGN KEY (ci_booklooker_id) REFERENCES ${_table}(bl_id);

--
-- Done
--
EOF

rm -f tmp.sql
