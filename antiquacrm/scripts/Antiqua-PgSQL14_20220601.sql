--
-- PostgreSQL database dump
--

-- Dumped from database version 14.1
-- Dumped by pg_dump version 14.2

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

DROP DATABASE IF EXISTS antiqua;
--
-- Name: antiqua; Type: DATABASE; Schema: -; Owner: -
--

CREATE DATABASE antiqua WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE = 'de_DE.UTF-8';


\connect antiqua

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: public; Type: SCHEMA; Schema: -; Owner: -
--

CREATE SCHEMA public;


--
-- Name: German; Type: COLLATION; Schema: public; Owner: -
--

CREATE COLLATION public."German" (provider = libc, locale = 'de_DE.utf8');


--
-- Name: func_character_column_info(character varying, character varying); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.func_character_column_info(tbn character varying, col character varying) RETURNS TABLE(column_name character varying, is_nullable character varying, character_maximum_length smallint)
    LANGUAGE sql
    AS $$
SELECT column_name,is_nullable,character_maximum_length
FROM   information_schema.columns
WHERE  table_name = tbn AND data_type = 'character varying' AND column_name = col
ORDER  BY ordinal_position;
$$;


--
-- Name: func_fetch_storage_id(character varying); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.func_fetch_storage_id(character varying) RETURNS smallint
    LANGUAGE sql
    AS $_$
SELECT sl_id::smallint FROM ref_storage_location WHERE sl_storage ILIKE $1;
$_$;


--
-- Name: func_find_inventory_articleid(integer); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.func_find_inventory_articleid(_id integer, OUT aid integer) RETURNS integer
    LANGUAGE sql
    AS $$
  SELECT
    COALESCE(b.ib_id,p.ip_id) AS aid
  FROM inventory AS i
  LEFT JOIN inventory_books AS b ON i.i_id=b.ib_id
  LEFT JOIN inventory_prints AS p ON i.i_id=p.ip_id
  WHERE i.i_id=_id AND (b.ib_count>0 OR p.ip_count>0);
$$;


--
-- Name: func_postalcode_table(character); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.func_postalcode_table(character) RETURNS text
    LANGUAGE sql
    AS $_$
    SELECT p_table::text FROM ui_postalcodes WHERE p_iso639_1 ILIKE $1;
$_$;


--
-- Name: func_shipping_address(integer); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.func_shipping_address(cid integer) RETURNS text
    LANGUAGE plpgsql
    AS $$
  DECLARE address TEXT;
BEGIN
  SELECT c_shipping_address INTO STRICT address FROM public.customers WHERE c_id=cid;
  IF length(address) < 1 THEN
    SELECT c_postal_address INTO STRICT address FROM public.customers WHERE c_id=cid;
  END IF;
  RETURN address;
END;
$$;


--
-- Name: new_book_article(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.new_book_article() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
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
$$;


--
-- Name: new_invoice_id(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.new_invoice_id() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  IF TG_OP = 'INSERT' THEN
    INSERT INTO public.invoice (in_since) VALUES (CURRENT_TIMESTAMP);
  END IF;

  NEW.o_invoice_id := currval('public.invoice_id_seq');

  IF NEW.o_invoice_id IS NULL THEN
    RAISE EXCEPTION 'o_invoice_id cannot be empty';
  END IF;

  RETURN NEW;
END;
$$;


--
-- Name: new_print_article(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.new_print_article() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  IF TG_OP = 'INSERT' THEN
    INSERT INTO public.inventory (i_since) VALUES (CURRENT_TIMESTAMP);
  END IF;

  NEW.ip_id := currval('public.inventory_id_seq');

  IF NEW.ip_id IS NULL THEN
    RAISE EXCEPTION 'ip_id cannot be empty';
  END IF;

  RETURN NEW;
END;
$$;


--
-- Name: payment_insert(integer, integer); Type: PROCEDURE; Schema: public; Owner: -
--

CREATE PROCEDURE public.payment_insert(IN _aid integer, IN _count integer)
    LANGUAGE sql
    AS $$
  UPDATE inventory_books  SET ib_count=(ib_count - _count),ib_changed=CURRENT_TIMESTAMP WHERE (ib_id=_aid AND ib_count!=0);
  UPDATE inventory_prints SET ip_count=(ip_count - _count),ip_changed=CURRENT_TIMESTAMP WHERE (ip_id=_aid AND ip_count!=0);
$$;


--
-- Name: payment_insert_trigger(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.payment_insert_trigger() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  IF (TG_OP = 'INSERT') THEN
    NEW.a_payment_id := currval('public.article_orders_id_seq');
    PERFORM NEW.a_article_id, NEW.a_count FROM article_orders WHERE a_payment_id=NEW.a_payment_id;
  END IF;

  IF NEW.a_article_id IS NULL THEN
    RAISE EXCEPTION 'a_article_id cannot be empty';
  END IF;

  IF NEW.a_count IS NULL THEN
    RAISE EXCEPTION 'a_count cannot be empty';
  END IF;

  CALL payment_insert(NEW.a_article_id,NEW.a_count);
  RETURN NULL;
END;
$$;


--
-- Name: article_orders_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.article_orders_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    MAXVALUE 2147483647
    CACHE 1;


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: article_orders; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.article_orders (
    a_payment_id integer DEFAULT nextval('public.article_orders_id_seq'::regclass) NOT NULL,
    a_order_id integer NOT NULL,
    a_article_id integer NOT NULL,
    a_customer_id integer NOT NULL,
    a_count smallint DEFAULT 0,
    a_title character varying(80) NOT NULL,
    a_price numeric(6,2) DEFAULT 0.00,
    a_sell_price numeric(6,2) DEFAULT 0.00,
    a_modified timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    a_provider_id character varying(20) DEFAULT NULL::character varying
);


--
-- Name: company_settings; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.company_settings (
    s_parameter character varying(60) NOT NULL,
    s_val1 boolean,
    s_val2 path,
    s_val3 character varying(128),
    s_val4 text
);


--
-- Name: customers_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.customers_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    MAXVALUE 2147483647
    CACHE 1;


--
-- Name: customers; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.customers (
    c_id integer DEFAULT nextval('public.customers_id_seq'::regclass) NOT NULL,
    c_locked boolean DEFAULT false,
    c_trusted smallint DEFAULT 0,
    c_transactions smallint DEFAULT 0,
    c_purchases smallint DEFAULT 0,
    c_company boolean DEFAULT false,
    c_company_name character varying(80),
    c_company_employer character varying(80),
    c_gender smallint DEFAULT 0,
    c_title character varying(25),
    c_firstname character varying(80) NOT NULL,
    c_lastname character varying(80) NOT NULL,
    c_postalcode character varying(10) NOT NULL,
    c_country character varying(100) NOT NULL,
    c_location character varying(80) NOT NULL,
    c_street character varying(80) NOT NULL,
    c_email_0 character varying(80),
    c_email_1 character varying(80),
    c_shipping_email character varying(80),
    c_website character varying(80),
    c_phone_0 character varying(21),
    c_phone_1 character varying(21),
    c_mobil_0 character varying(21),
    c_mobil_1 character varying(21),
    c_fax_0 character varying(21),
    c_postal_address text,
    c_shipping_address text,
    c_comments text,
    c_iban character varying(34),
    c_swift_bic character varying(11),
    c_tax_id character varying(34),
    c_since timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    c_changed timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


--
-- Name: extern_categories; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.extern_categories (
    c_id smallint NOT NULL,
    c_zvab smallint DEFAULT 0,
    c_bf smallint DEFAULT 0,
    c_main boolean DEFAULT false,
    c_name character varying(80)
);


--
-- Name: extern_categories_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.extern_categories_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    MAXVALUE 32767
    CACHE 1;


--
-- Name: extern_categories_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.extern_categories_id_seq OWNED BY public.extern_categories.c_id;


--
-- Name: inventory; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.inventory (
    i_id integer NOT NULL,
    i_since timestamp without time zone NOT NULL
);


--
-- Name: inventory_books; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.inventory_books (
    ib_id integer NOT NULL,
    ib_title character varying(80) NOT NULL,
    ib_title_extended character varying(148),
    ib_author character varying(128) NOT NULL,
    ib_publisher character varying(128),
    ib_keyword character varying(60),
    ib_year numeric(4,0),
    ib_condition character varying(128),
    ib_designation character varying(128),
    ib_description text,
    ib_edition smallint DEFAULT 0,
    ib_isbn bigint,
    ib_storage smallint DEFAULT 0,
    ib_pagecount integer DEFAULT 0,
    ib_count smallint DEFAULT 0,
    ib_signed boolean DEFAULT false,
    ib_language character(5) DEFAULT 'de_DE'::character(5) NOT NULL,
    ib_price numeric(6,2) DEFAULT 0.00,
    ib_weight integer DEFAULT 0,
    ib_restricted boolean DEFAULT false,
    ib_volume smallint DEFAULT 0,
    ib_changed timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    ib_internal_description text
);


--
-- Name: inventory_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

ALTER TABLE public.inventory ALTER COLUMN i_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.inventory_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: inventory_images; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.inventory_images (
    im_id integer NOT NULL,
    im_imgdata text NOT NULL,
    im_changed timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


--
-- Name: inventory_orders_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.inventory_orders_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    MAXVALUE 2147483647
    CACHE 1;


--
-- Name: inventory_orders; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.inventory_orders (
    o_id integer DEFAULT nextval('public.inventory_orders_id_seq'::regclass) NOT NULL,
    o_order_status smallint DEFAULT 0,
    o_payment_status boolean DEFAULT false,
    o_customer_id integer NOT NULL,
    o_invoice_id integer NOT NULL,
    o_delivery_service smallint NOT NULL,
    o_delivery_send_id character varying(20),
    o_delivery character varying(20),
    o_provider_name character varying(80) NOT NULL,
    o_provider_order_id character varying(20) NOT NULL,
    o_provider_order text,
    o_locked boolean DEFAULT false,
    o_closed boolean DEFAULT false,
    o_notify boolean DEFAULT false,
    o_modified timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    o_since timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


--
-- Name: inventory_prints; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.inventory_prints (
    ip_id integer NOT NULL,
    ip_count smallint DEFAULT 0,
    ip_title character varying(80),
    ip_title_extended character varying(148),
    ip_author character varying(128),
    ip_year numeric(4,0) DEFAULT 0,
    ip_views boolean DEFAULT false,
    ip_kolorit boolean DEFAULT false,
    ip_technique smallint DEFAULT 0,
    ip_format character varying(50),
    ip_landscape boolean DEFAULT false,
    ip_condition character varying(128),
    ip_designation character varying(128),
    ip_price numeric(6,2) DEFAULT 0.00,
    ip_description text,
    ip_restricted boolean DEFAULT false,
    ip_changed timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    ip_internal_description text,
    ip_storage smallint
);


--
-- Name: invoice; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.invoice (
    in_id integer NOT NULL,
    in_since timestamp without time zone NOT NULL
);


--
-- Name: invoice_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

ALTER TABLE public.invoice ALTER COLUMN in_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.invoice_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: ref_book_formats; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_book_formats (
    rbf_id smallint NOT NULL,
    rbf_format character varying(80) NOT NULL,
    rbf_description character varying(256),
    rbf_simple character varying(50)
);


--
-- Name: ref_book_formats_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

ALTER TABLE public.ref_book_formats ALTER COLUMN rbf_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.ref_book_formats_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: ref_delivery_service_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.ref_delivery_service_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    MAXVALUE 2147483647
    CACHE 1;


--
-- Name: ref_delivery_service; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_delivery_service (
    d_id integer DEFAULT nextval('public.ref_delivery_service_id_seq'::regclass) NOT NULL,
    d_name character varying(30),
    d_website character varying(100),
    d_since timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL
);


--
-- Name: ref_postalcodes_at; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_postalcodes_at (
    p_plz integer NOT NULL,
    p_idn integer,
    p_location character varying(256),
    p_state character varying(256)
);


--
-- Name: ref_postalcodes_ch; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_postalcodes_ch (
    p_plz integer NOT NULL,
    p_location character varying(30),
    p_state character varying(30),
    p_acronym character varying(3),
    p_iso_3166_2 character varying(5)
);


--
-- Name: ref_postalcodes_de; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_postalcodes_de (
    p_plz integer NOT NULL,
    p_location character varying(256) NOT NULL,
    p_state character varying(256) NOT NULL,
    p_acronym character(2),
    p_iso_3166_2 character(5),
    p_ags smallint
);


--
-- Name: ref_print_technique; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_print_technique (
    rpt_id smallint NOT NULL,
    rpt_type character varying(80) NOT NULL,
    rpt_description text
);


--
-- Name: ref_print_technique_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

ALTER TABLE public.ref_print_technique ALTER COLUMN rpt_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.ref_print_technique_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: ref_storage_keywords_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.ref_storage_keywords_id_seq
    START WITH 1
    INCREMENT BY 1
    MINVALUE 0
    MAXVALUE 32767
    CACHE 1;


--
-- Name: ref_storage_keywords; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_storage_keywords (
    sl_ident smallint DEFAULT nextval('public.ref_storage_keywords_id_seq'::regclass) NOT NULL,
    sl_id smallint NOT NULL,
    sl_keyword character varying(50) NOT NULL
);


--
-- Name: ref_storage_location; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ref_storage_location (
    sl_id smallint NOT NULL,
    sl_storage character varying(4) NOT NULL,
    sl_identifier character varying(50) NOT NULL,
    sl_location character varying(30)
);


--
-- Name: ref_storage_location_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

ALTER TABLE public.ref_storage_location ALTER COLUMN sl_id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.ref_storage_location_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: ui_autofill_keywords; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ui_autofill_keywords (
    k_table_cell character varying(60) NOT NULL,
    k_keyword character varying(80) NOT NULL,
    k_description text
);


--
-- Name: ui_postalcodes; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ui_postalcodes (
    p_id smallint NOT NULL,
    p_country character varying(128) NOT NULL,
    p_iso639_1 character varying(6) NOT NULL,
    p_table character varying(128) NOT NULL
);


--
-- Name: ui_postalcodes_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

ALTER TABLE public.ui_postalcodes ALTER COLUMN p_id ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.ui_postalcodes_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: extern_categories c_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.extern_categories ALTER COLUMN c_id SET DEFAULT nextval('public.extern_categories_id_seq'::regclass);


--
-- Name: article_orders article_order_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.article_orders
    ADD CONSTRAINT article_order_id UNIQUE (a_payment_id);


--
-- Name: article_orders article_orders_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.article_orders
    ADD CONSTRAINT article_orders_pkey PRIMARY KEY (a_payment_id);


--
-- Name: extern_categories categories_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.extern_categories
    ADD CONSTRAINT categories_pkey PRIMARY KEY (c_id);


--
-- Name: customers customer_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customer_id UNIQUE (c_id);


--
-- Name: ref_delivery_service delivery_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_delivery_service
    ADD CONSTRAINT delivery_id UNIQUE (d_id);


--
-- Name: ref_delivery_service delivery_service_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_delivery_service
    ADD CONSTRAINT delivery_service_pkey PRIMARY KEY (d_id);


--
-- Name: ref_book_formats format_title; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_book_formats
    ADD CONSTRAINT format_title UNIQUE (rbf_format);


--
-- Name: inventory i_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory
    ADD CONSTRAINT i_id UNIQUE (i_id);


--
-- Name: inventory_images image_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_images
    ADD CONSTRAINT image_id UNIQUE (im_id);


--
-- Name: invoice in_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.invoice
    ADD CONSTRAINT in_id UNIQUE (in_id);


--
-- Name: inventory_books inventory_book_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_books
    ADD CONSTRAINT inventory_book_pkey PRIMARY KEY (ib_id);


--
-- Name: inventory_orders inventory_orders_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_orders
    ADD CONSTRAINT inventory_orders_pkey PRIMARY KEY (o_id);


--
-- Name: inventory inventory_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory
    ADD CONSTRAINT inventory_pkey PRIMARY KEY (i_id);


--
-- Name: inventory_prints inventory_prints_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_prints
    ADD CONSTRAINT inventory_prints_pkey PRIMARY KEY (ip_id);


--
-- Name: invoice invoice_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.invoice
    ADD CONSTRAINT invoice_pkey PRIMARY KEY (in_id);


--
-- Name: inventory_orders order_id; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_orders
    ADD CONSTRAINT order_id UNIQUE (o_id);


--
-- Name: ui_postalcodes postalcodes_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ui_postalcodes
    ADD CONSTRAINT postalcodes_pkey PRIMARY KEY (p_id);


--
-- Name: ref_print_technique ref_print_technique_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_print_technique
    ADD CONSTRAINT ref_print_technique_pkey PRIMARY KEY (rpt_id);


--
-- Name: ref_delivery_service service_name; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_delivery_service
    ADD CONSTRAINT service_name UNIQUE (d_name);


--
-- Name: ref_storage_location storage_location_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_storage_location
    ADD CONSTRAINT storage_location_pkey PRIMARY KEY (sl_id);


--
-- Name: ref_storage_location storage_position; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_storage_location
    ADD CONSTRAINT storage_position UNIQUE (sl_storage);


--
-- Name: ref_print_technique technique; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_print_technique
    ADD CONSTRAINT technique UNIQUE (rpt_type);


--
-- Name: company_settings ui_parameter; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.company_settings
    ADD CONSTRAINT ui_parameter UNIQUE (s_parameter);


--
-- Name: company_settings ui_settings_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.company_settings
    ADD CONSTRAINT ui_settings_pkey PRIMARY KEY (s_parameter);


--
-- Name: ref_storage_keywords uniq_keywords; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_storage_keywords
    ADD CONSTRAINT uniq_keywords UNIQUE (sl_keyword);


--
-- Name: ui_postalcodes unique_country; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ui_postalcodes
    ADD CONSTRAINT unique_country UNIQUE (p_country);


--
-- Name: ui_postalcodes unique_iso639_1; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ui_postalcodes
    ADD CONSTRAINT unique_iso639_1 UNIQUE (p_iso639_1);


--
-- Name: ui_autofill_keywords unique_keywords; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ui_autofill_keywords
    ADD CONSTRAINT unique_keywords UNIQUE (k_keyword);


--
-- Name: extern_categories unique_names; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.extern_categories
    ADD CONSTRAINT unique_names UNIQUE (c_name);


--
-- Name: uniq_customer; Type: INDEX; Schema: public; Owner: -
--

CREATE UNIQUE INDEX uniq_customer ON public.customers USING btree (c_firstname, c_lastname, c_postalcode, c_location);


--
-- Name: article_orders after_insert_trigger; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER after_insert_trigger AFTER INSERT ON public.article_orders FOR EACH ROW EXECUTE FUNCTION public.payment_insert_trigger();


--
-- Name: inventory_books new_book_article; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER new_book_article BEFORE INSERT ON public.inventory_books FOR EACH ROW EXECUTE FUNCTION public.new_book_article();


--
-- Name: inventory_orders new_invoice_id; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER new_invoice_id BEFORE INSERT ON public.inventory_orders FOR EACH ROW EXECUTE FUNCTION public.new_invoice_id();


--
-- Name: inventory_prints new_print_article; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER new_print_article BEFORE INSERT ON public.inventory_prints FOR EACH ROW EXECUTE FUNCTION public.new_print_article();


--
-- Name: inventory_books article_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_books
    ADD CONSTRAINT article_id FOREIGN KEY (ib_id) REFERENCES public.inventory(i_id);


--
-- Name: inventory_prints article_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_prints
    ADD CONSTRAINT article_id FOREIGN KEY (ip_id) REFERENCES public.inventory(i_id);


--
-- Name: article_orders article_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.article_orders
    ADD CONSTRAINT article_id FOREIGN KEY (a_article_id) REFERENCES public.inventory(i_id);


--
-- Name: inventory_images article_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_images
    ADD CONSTRAINT article_id FOREIGN KEY (im_id) REFERENCES public.inventory(i_id);


--
-- Name: article_orders customer_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.article_orders
    ADD CONSTRAINT customer_id FOREIGN KEY (a_customer_id) REFERENCES public.customers(c_id);


--
-- Name: inventory_orders delivery_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_orders
    ADD CONSTRAINT delivery_id FOREIGN KEY (o_delivery_service) REFERENCES public.ref_delivery_service(d_id);


--
-- Name: inventory_prints foreign_technique_key; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_prints
    ADD CONSTRAINT foreign_technique_key FOREIGN KEY (ip_technique) REFERENCES public.ref_print_technique(rpt_id);


--
-- Name: inventory_books storage_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_books
    ADD CONSTRAINT storage_id FOREIGN KEY (ib_storage) REFERENCES public.ref_storage_location(sl_id);


--
-- Name: inventory_prints storage_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.inventory_prints
    ADD CONSTRAINT storage_id FOREIGN KEY (ip_storage) REFERENCES public.ref_storage_location(sl_id);


--
-- Name: ref_storage_keywords storage_id; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ref_storage_keywords
    ADD CONSTRAINT storage_id FOREIGN KEY (sl_id) REFERENCES public.ref_storage_location(sl_id);


--
-- Name: DATABASE antiqua; Type: ACL; Schema: -; Owner: -
--

GRANT ALL ON DATABASE antiqua TO antiqua;


--
-- PostgreSQL database dump complete
--

