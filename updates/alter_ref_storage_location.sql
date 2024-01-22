--
-- incrase  charset for column sl_storage
--
DROP VIEW IF EXISTS public.view_books_from_open_orders;
DROP VIEW IF EXISTS public.view_group_by_keywords;
DROP VIEW IF EXISTS public.view_no_book_picture;
DROP VIEW IF EXISTS public.view_no_print_picture;

ALTER TABLE ref_storage_location ALTER COLUMN sl_storage TYPE character varying(20);

CREATE VIEW public.view_no_print_picture AS
 SELECT inventory_prints.ip_id AS articleid,
    inventory_prints.ip_title AS title,
    inventory_prints.ip_title_extended AS categorie,
    ref_storage_location.sl_storage AS storages
   FROM (((public.inventory
     LEFT JOIN public.inventory_prints ON ((inventory.i_id = inventory_prints.ip_id)))
     LEFT JOIN public.inventory_images ON ((inventory_images.im_id = inventory.i_id)))
     LEFT JOIN public.ref_storage_location ON ((ref_storage_location.sl_id = inventory_prints.ip_storage)))
  WHERE ((inventory.i_id > 0) AND (inventory_images.im_id IS NULL) AND (inventory_prints.ip_id IS NOT NULL) AND (inventory_prints.ip_count > 0))
  ORDER BY ref_storage_location.sl_storage;

COMMENT ON VIEW public.view_no_print_picture IS 'Zeige mir alle Drucke, Stiche und Lithografien ohne Bild';

CREATE VIEW public.view_no_book_picture AS
 SELECT inventory_books.ib_id AS articleid,
    inventory_books.ib_title AS title,
    inventory_books.ib_author AS author,
    ref_storage_location.sl_storage AS storages
   FROM (((public.inventory
     LEFT JOIN public.inventory_books ON ((inventory.i_id = inventory_books.ib_id)))
     LEFT JOIN public.inventory_images ON ((inventory_images.im_id = inventory.i_id)))
     LEFT JOIN public.ref_storage_location ON ((ref_storage_location.sl_id = inventory_books.ib_storage)))
  WHERE ((inventory.i_id > 0) AND (inventory_images.im_id IS NULL) AND (inventory_books.ib_id IS NOT NULL) AND (inventory_books.ib_count > 0))
  ORDER BY ref_storage_location.sl_storage;

COMMENT ON VIEW public.view_no_book_picture IS 'Zeige mir alle Bucheinträge ohne Vorschau Bild';

CREATE VIEW public.view_group_by_keywords AS
 SELECT ref_storage_location.sl_identifier AS kategorie,
    ref_storage_location.sl_storage AS lager,
    (sum(inventory_books.ib_price))::money AS preisvolumen,
    (avg(inventory_books.ib_price))::money AS durchschnitt,
    (count(inventory_books.ib_storage))::numeric AS bestand
   FROM (public.inventory_books
     RIGHT JOIN public.ref_storage_location ON ((ref_storage_location.sl_id = inventory_books.ib_storage)))
  WHERE (inventory_books.ib_count > 0)
  GROUP BY ref_storage_location.sl_identifier, ref_storage_location.sl_storage
  ORDER BY (count(inventory_books.ib_storage)) DESC;

COMMENT ON VIEW public.view_group_by_keywords IS 'Alle Kategorien gruppiert mit Schlüsselwort, zusätzlich Gesamtpreis dieser Kategorien.';

CREATE VIEW public.view_books_from_open_orders AS
 SELECT inventory_books.ib_id,
    inventory_books.ib_count,
    inventory_books.ib_title,
    inventory_books.ib_author,
    inventory_books.ib_publisher,
    inventory_books.ib_year,
    inventory_books.ib_price,
    ref_storage_location.sl_storage,
    inventory_books.ib_isbn,
    inventory_books.ib_changed,
        CASE
            WHEN (inventory_images.im_id IS NOT NULL) THEN true
            ELSE false
        END AS image_exists
   FROM ((((public.inventory_books
     LEFT JOIN public.article_orders ON ((article_orders.a_article_id = inventory_books.ib_id)))
     LEFT JOIN public.inventory_orders ON ((inventory_orders.o_id = article_orders.a_order_id)))
     LEFT JOIN public.ref_storage_location ON ((ref_storage_location.sl_id = inventory_books.ib_storage)))
     LEFT JOIN public.inventory_images ON ((inventory_images.im_id = inventory_books.ib_id)))
  WHERE (inventory_orders.o_order_status < 5)
  ORDER BY inventory_orders.o_since DESC;

COMMENT ON VIEW public.view_books_from_open_orders IS 'Zeige alle Bücher von den Aktuellen Aufträgen.';
