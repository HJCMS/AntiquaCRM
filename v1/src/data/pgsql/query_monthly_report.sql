-- @brief Query Monthly Reports
-- @file query_monthly_report.sql
-- o_delivered '2022-11-01T00:00:00' AND '2022-11-30T23:59:00'
SELECT DATE(o_delivered) AS date, lpad(o_id::text, 7, '0') AS invoice,
  lpad(a_article_id::text, 7, '0') AS article,
  a_sell_price::MONEY AS price,
  (CASE o_vat_included WHEN true THEN 'Ja' ELSE 'Nein' END) AS incl,
  concat(o_vat_levels::text,'%') AS vat,
  (CASE o_vat_included WHEN true
    THEN ROUND((a_sell_price * o_vat_levels) / (100 + o_vat_levels), 2)::MONEY
    ELSE ROUND((a_sell_price / (100 + o_vat_levels) * o_vat_levels), 2)::MONEY END) AS ust,
  (CASE o_delivery_add_price WHEN true
    THEN d_price::MONEY ELSE 0::MONEY END) AS porto,
  (CASE o_delivery_add_price WHEN true
    THEN (a_sell_price + d_price)::MONEY
    ELSE a_sell_price::MONEY END) AS total,
  (CASE o_delivery_add_price WHEN true
    THEN (a_sell_price + d_price)::NUMERIC
    ELSE a_sell_price::NUMERIC END) AS calc
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
LEFT JOIN ref_delivery_cost ON d_cid=o_delivery_package
WHERE o_payment_status=true AND o_order_status=5 @SQL_WHERE_CLAUSE@ ORDER BY o_delivered;
