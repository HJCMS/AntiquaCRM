-- @brief Default Query for Tab Orders
-- @file query_tab_orders_main.sql
SELECT o_id, o_since, o_order_status, o_payment_status,
 CASE WHEN c_company=true THEN c_company_name ELSE CONCAT((CASE WHEN length(c_title)>2 THEN CONCAT(c_title, ' ') ELSE '' END),c_firstname,' ',c_lastname,' (',c_country,')') END AS customer,
 d_name AS delivery_service, CONCAT_WS(': ', o_provider_name, o_provider_order_id) AS provider_info,
 (EXTRACT(EPOCH FROM timestamptz (CURRENT_TIMESTAMP)) - EXTRACT(EPOCH FROM timestamptz (o_since))) AS order_age, o_delivered
FROM inventory_orders
LEFT JOIN customers ON c_id=o_customer_id
LEFT JOIN ref_delivery_service ON d_id=o_delivery_service
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
