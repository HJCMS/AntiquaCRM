-- @brief Update Provider Treeview
-- @file query_provider_orders.sql
-- @see AntiquaCRM::OrderStatus
SELECT
 pr_name AS order_provider,
 pr_order AS order_number,
 COALESCE(o_order_status,0) AS order_status,
 COALESCE(pr_created,CURRENT_TIMESTAMP) AS order_datetime,
 (CASE WHEN length(pr_buyer) > 20 THEN CONCAT(LEFT(pr_buyer, 20),'...') ELSE pr_buyer END) AS order_buyername,
 pr_buyer AS order_comment,
 pr_order_accepted
FROM provider_orders
LEFT JOIN inventory_orders ON (o_provider_name=pr_name AND o_provider_order_id=pr_order)
WHERE pr_order_accepted=false OR (pr_order_accepted=true AND o_order_status<4)
 AND (o_since BETWEEN (CURRENT_TIMESTAMP - justify_interval(interval '1 month')) AND CURRENT_TIMESTAMP)
ORDER BY (pr_name,pr_order,o_since);
