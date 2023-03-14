-- @brief Search Order Payment Information
-- @file query_printing_billing_info.sql
-- @note vat_level 2=Reduziert, 1=Standard
SELECT a_article_id AS aid, a_count AS quant, a_sell_price AS sellprice, o_vat_included, o_delivery,
  (CASE WHEN i_id=ib_id THEN 2 ELSE 1 END ) AS vat_level, a_type,
  GREATEST(ib_title,ip_title,cv_title,va_title,NULL) AS title,
  (CASE WHEN o_delivery_add_price=true THEN d_price ELSE 0.00 END) AS packageprice
FROM inventory
LEFT JOIN article_orders ON (a_article_id=i_id)
LEFT JOIN inventory_orders ON (o_id=a_order_id AND o_customer_id=a_customer_id)
LEFT JOIN inventory_books ON (ib_id=i_id AND ib_id=a_article_id)
LEFT JOIN inventory_prints ON (ip_id=i_id AND ip_id=a_article_id)
LEFT JOIN inventory_cdvinyl ON (cv_id=i_id AND cv_id=a_article_id)
LEFT JOIN inventory_various ON (va_id=i_id AND va_id=a_article_id)
LEFT JOIN ref_delivery_cost ON (d_srv=o_delivery_service AND d_cid=o_delivery_package)
WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_article_id;
