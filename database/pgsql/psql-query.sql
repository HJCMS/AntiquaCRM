
DROP TABLE IF EXISTS public.article_orders;

CREATE TABLE public.article_orders (
  a_order_id integer NOT NULL,
  a_article_id integer NOT NULL,
  a_costumer_id integer NOT NULL,
  a_title character varying(80) NOT NULL,
  a_price numeric(6,2) DEFAULT 0.00,
  a_sell_price numeric(6,2) DEFAULT 0.00,
  a_modified timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);

COMMENT ON TABLE public.article_orders IS 'Auftraege System';

COMMENT ON COLUMN public.article_orders.a_order_id IS 'Auftrags ID';

COMMENT ON COLUMN public.article_orders.a_article_id IS 'Artikel ID';

COMMENT ON COLUMN public.article_orders.a_costumer_id IS 'Kunden ID';

COMMENT ON COLUMN public.article_orders.a_title IS 'Artikel Titel';

COMMENT ON COLUMN public.article_orders.a_price IS 'Artikel Preis';

COMMENT ON COLUMN public.article_orders.a_sell_price IS 'Artikel Verkaufs Preis';

COMMENT ON COLUMN public.article_orders.a_modified IS 'Aenderungen';

ALTER TABLE ONLY public.article_orders
  ADD CONSTRAINT order_id FOREIGN KEY (a_order_id) REFERENCES public.inventory_orders(o_id);

ALTER TABLE ONLY public.article_orders
  ADD CONSTRAINT article_id FOREIGN KEY (a_article_id) REFERENCES public.inventory(i_id);

ALTER TABLE ONLY public.article_orders
  ADD CONSTRAINT costumer_id FOREIGN KEY (a_costumer_id) REFERENCES public.costumers(c_id);
