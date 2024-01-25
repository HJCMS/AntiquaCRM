-- file://abe-export.sql
-- type: psql
-- comment: PostgreSQL
-- https://verkaeuferhilfe.abebooks.de/2009/09/die-abebooks-standardkonvertierung.html
-----------------------------------------------------------
-- INVENTORY BOOKS
-- pg-heinemann -c "\dS inventory_books"
SELECT ib_id AS Artikelnummer,
  COALESCE(ib_price, 0.00) AS Preis,
  ib_count AS Anzahl,
  CONCAT(ib_title,' ',ib_title_extended) AS Titel,
  (CASE ib_author WHEN 'NOTE_SET' THEN '' ELSE ib_author END) AS Autor,
  '' AS Illustrator,
  ib_category_main AS ABEKategorie,
  ib_category_sub AS Katalog1,
  sl_identifier AS Katalog2,
  ib_category_main AS Katalog3,
  ib_publisher AS Verlag,
  '' AS Ort,
  ib_year AS Jahr,
  ib_isbn AS ISBN,
  '' AS Format,
  (CASE ib_weight WHEN 0 THEN '' ELSE CONCAT(ib_weight,'g') END) AS Gewicht,
  'Buch' AS Produktart,
  '' AS Druck,
  TRANSLATE(CONCAT(ib_description,' ',ib_designation), chr(9)||chr(10)||chr(11)||chr(13),' ') AS Beschreibung,
  ib_keyword AS Schlagworte,
  ib_designation AS Einband,
  (CASE ib_condition WHEN 1 THEN 'sehr gut' WHEN 2 THEN 'gut' ELSE 'befriedigend' END) AS Zustand,
  ib_designation AS Schutzumschlag,
  (CASE ib_edition WHEN 0 THEN '' ELSE CONCAT(ib_edition,'. Auflage') END) AS Auflage,
  (CASE ib_signed WHEN true THEN 'Signiert' ELSE '' END) AS Signiert,
  (CASE COALESCE(im_id,0) WHEN 0 THEN '' ELSE CONCAT(im_id,'.jpg') END) AS Bild,
  7 AS MWST,
  'EUR' AS Waehrung,
  '' AS AmazonZustand
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN ref_storage_location ON sl_id=ib_storage
LEFT JOIN inventory_images ON im_id=i_id
WHERE (ib_count>0) AND (ib_price>0) AND (ib_title IS NOT NULL) AND ib_restricted=false
ORDER BY i_id ASC;

-- INVENTORY PRINTS
-- pg-heinemann -c "\dS inventory_prints"
SELECT ip_id AS Artikelnummer,
  COALESCE(ip_price, 0.00) AS Preis,
  ip_count AS Anzahl,
  CONCAT(ip_title,' ',ip_title_extended) AS Titel,
  (CASE ip_author WHEN 'NOTE_SET' THEN '' ELSE ip_author END) AS Autor,
  '' AS Illustrator,
  'Kunst' AS ABEKategorie,
  'Grafik/Stiche/Lithographien' AS Katalog1,
  (CASE ip_views WHEN true THEN 'Ansichten' ELSE '' END) AS Katalog2,
  (CASE rpt_type WHEN 'Unbekannt' THEN '' ELSE rpt_type END) AS Katalog3,
  '' AS Verlag,
  '' AS Ort,
  ip_year AS Jahr,
  '' AS ISBN,
  ip_format AS Format,
  '' AS Gewicht,
  'Druckgrafik' AS Produktart,
  '' AS Druck,
  TRANSLATE(ip_description, chr(9)||chr(10)||chr(11)||chr(13),' ') AS Beschreibung,
  (CASE ip_landscape WHEN true THEN 'Landscape,Querformat' ELSE 'Porträt,Hochformat' END) AS Schlagworte,
  ip_condition AS Einband,
  CONCAT('Zustand: ',COALESCE(ip_condition,'befriedigend')) AS Zustand,
  ip_designation AS Schutzumschlag,
  '' AS Auflage,
  '' AS Signiert,
  (CASE COALESCE(im_id,0) WHEN 0 THEN '' ELSE CONCAT(im_id,'.jpg') END) AS Bild,
  7 AS MWST,
  'EUR' AS Waehrung,
  '' AS AmazonZustand
FROM inventory
LEFT JOIN inventory_prints ON ip_id=i_id
LEFT JOIN ref_storage_location ON sl_id=ip_storage
LEFT JOIN ref_print_technique ON rpt_id=ip_technique
LEFT JOIN inventory_images ON im_id=i_id
WHERE (ip_count>0) AND (ip_price>0) AND (ip_title IS NOT NULL) AND ip_restricted=false
ORDER BY i_id ASC;

--
-- EOF
--
