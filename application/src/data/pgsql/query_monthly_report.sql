-- @brief Query Monthly Reports
-- @file query_monthly_report.sql
-- a_tax : 'Umsatzsteuer Type 0=Normal 1=Reduziert'
SELECT DATE(o_delivered) AS date,
  lpad(o_id::text, 7, '0') AS invoice,
  lpad(a_article_id::text, 7, '0') AS article,
  a_sell_price::MONEY AS price,
  (CASE o_vat_included WHEN true THEN 'Ja' ELSE 'Nein' END) AS incl,
  CONCAT(st_tax_value::text, '%') AS sales_tax_type,
  (CASE o_vat_included WHEN true
    THEN ROUND((a_sell_price * st_tax_value) / (100 + st_tax_value), 2)::MONEY
    ELSE ROUND((a_sell_price / (100 + st_tax_value) * st_tax_value), 2)::MONEY
    END) AS sales_tax_calc,
  (CASE o_delivery_add_price WHEN true
    THEN d_price::MONEY
    ELSE a_refunds_cost::MONEY
    END) AS porto,
  (CASE o_delivery_add_price WHEN true
    THEN (a_sell_price + d_price)::MONEY
    ELSE a_sell_price::MONEY
    END) AS total,
  (CASE o_delivery_add_price WHEN true
    THEN (a_sell_price + d_price)::NUMERIC
    ELSE a_sell_price::NUMERIC
    END) AS calc,
  a_refunds_cost AS refundscost
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
LEFT JOIN ref_delivery_cost ON d_cid=o_delivery_package
LEFT JOIN ref_sales_tax ON st_tax_type=a_tax
WHERE ((o_payment_status=1 AND o_order_status=4) OR (o_order_status=6 AND o_payment_status=4))
 AND @SQL_WHERE_CLAUSE@ AND date_part('month', a_modified)=date_part('month',o_delivered)
ORDER BY o_delivered;
