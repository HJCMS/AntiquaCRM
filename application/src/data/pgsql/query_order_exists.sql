-- @brief Search existing Article Orders
-- @file query_order_exists.sql
-- @test o_provider_name ILIKE 'AbeBooks' AND o_provider_order_id='653664115'
SELECT o_id,
  CONCAT_WS(':', o_provider_name, o_provider_order_id) AS prinfo,
  TRIM(CONCAT_WS(' ', c_title, c_firstname, c_lastname)) AS buyer
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
LEFT JOIN customers ON c_id=o_customer_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY o_since;
