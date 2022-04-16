-- \copy { table [ ( column_list ) ] | ( query ) } to { 'filename' | program 'command' | stdout | pstdout } [ [ with ] ( option [, ...] ) ]
\copy table (BuchID,BildID,Titel,Zeit)
(SELECT b.ib_id AS BuchID,i.im_id AS BildID,b.ib_title AS Titel,b.ib_changed AS Zeit
FROM inventory_books AS b LEFT JOIN inventory_images AS i ON i.im_id=b.ib_id
WHERE i.im_id IS NOT NULL ORDER BY b.ib_id)

-- NOT Working
-- TO STDOUT WITH CSV DELIMITER '|' HEADER;

-- Works from Client
-- \copy ui_autofill_keywords TO STDOUT  WITH CSV DELIMITER '|' HEADER;
