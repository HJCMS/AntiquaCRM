-- \copy { table [ ( column_list ) ] | ( query ) } to { 'filename' | program 'command' | stdout | pstdout } [ [ with ] ( option [, ...] ) ]
--\copy table (BuchID,BildID,Titel,Zeit)
--(SELECT b.ib_id AS BuchID,i.im_id AS BildID,b.ib_title AS Titel,b.ib_changed AS Zeit
--FROM inventory_books AS b LEFT JOIN inventory_images AS i ON i.im_id=b.ib_id
--WHERE i.im_id IS NOT NULL ORDER BY b.ib_id)
-- NOT Working
-- TO STDOUT WITH CSV DELIMITER '|' HEADER;

ALTER TABLE public.ui_autofill_keywords ADD CONSTRAINT unique_keywords UNIQUE (k_keyword);

-- Works from Client
-- \copy ui_autofill_keywords TO STDOUT  WITH CSV DELIMITER '|' HEADER;
-- \df new_articleid

SELECT pg_catalog.setval('public.inventory_id_seq', (SELECT MAX(i_id) FROM public.inventory), true);

DROP TRIGGER IF EXISTS new_articleid ON inventory_books;
DROP FUNCTION IF EXISTS new_articleid();

CREATE FUNCTION public.new_articleid()
  RETURNS trigger
  LANGUAGE 'plpgsql'
  VOLATILE
AS $BODY$
BEGIN
  IF TG_OP = 'INSERT' THEN
    INSERT INTO public.inventory (i_since) VALUES (CURRENT_TIMESTAMP);
  END IF;

  NEW.ib_id := currval('public.inventory_id_seq');

  IF NEW.ib_id IS NULL THEN
    RAISE EXCEPTION 'ib_id cannot be empty';
  END IF;

  RETURN NEW;
END;
$BODY$;

CREATE TRIGGER new_articleid BEFORE INSERT ON inventory_books FOR EACH ROW EXECUTE PROCEDURE new_articleid();

SELECT MAX(i_id) FROM public.inventory;

INSERT INTO public.inventory_books (ib_title,ib_author,ib_language) VALUES ('test','test','en_GB');

SELECT MAX(i_id) FROM public.inventory;
