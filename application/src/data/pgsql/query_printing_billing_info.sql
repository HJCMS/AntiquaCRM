-- @brief Search Order Payment Information
-- @file query_printing_billing_info.sql
SELECT a_article_id AS aid, a_count AS quant, a_sell_price AS sellprice,
  COALESCE(ib_title,ip_title) AS title, o_vat_included, o_delivery, o_vat_levels,
  (CASE WHEN o_delivery_add_price=true THEN d_price ELSE 0.00 END) AS packageprice
FROM inventory
LEFT JOIN article_orders ON (a_article_id=i_id)
LEFT JOIN inventory_orders ON (o_id=a_order_id AND o_customer_id=a_customer_id)
LEFT JOIN inventory_books ON (ib_id=i_id AND ib_id=a_article_id)
LEFT JOIN inventory_prints ON (ip_id=i_id AND ip_id=a_article_id)
LEFT JOIN ref_delivery_cost ON (d_srv=o_delivery_service AND d_cid=o_delivery_package)
WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_article_id;
