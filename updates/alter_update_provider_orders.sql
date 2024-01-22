--
-- Adding pr_ignore=false to revert ignored entries from provider-orders when create an new order
--
DROP FUNCTION IF EXISTS provider_order_accepted CASCADE;

CREATE FUNCTION public.provider_order_accepted() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  IF TG_OP = 'INSERT' AND NEW.o_id > 0 AND NEW.o_provider_order_id IS NOT NULL THEN
    -- RAISE NOTICE 'UPDATE provider_orders SET pr_order_id=% WHERE pr_order=%', NEW.o_id, NEW.o_provider_order_id;
    UPDATE public.provider_orders SET pr_order_id=NEW.o_id, pr_order_accepted=true, pr_ignore=false WHERE pr_order=NEW.o_provider_order_id;
    UPDATE public.inventory_orders SET o_delivery=CONCAT(DATE_PART('year',CURRENT_DATE),DATE_PART('doy',CURRENT_DATE),NEW.o_id) WHERE o_id=NEW.o_id;
  END IF;
  RETURN NULL;
END;
$$;

CREATE TRIGGER update_provider_orders AFTER INSERT ON public.inventory_orders FOR EACH ROW EXECUTE FUNCTION public.provider_order_accepted();
