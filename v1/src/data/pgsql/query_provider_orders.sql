-- @brief Update Provider Treeview
-- @file query_provider_orders.sql
-- @see AntiquaCRM::OrderStatus
SELECT
 pr_name AS order_provider,
 pr_order AS order_number,
 COALESCE(o_order_status,0) AS order_status,
 COALESCE(pr_created,CURRENT_TIMESTAMP) AS order_datetime,
 (CASE WHEN length(pr_buyer) > 20 THEN CONCAT(LEFT(pr_buyer, 20),'...') ELSE pr_buyer END) AS order_buyername,
 pr_buyer AS order_comment
FROM provider_orders
LEFT JOIN inventory_orders ON (o_provider_name=pr_name AND o_provider_order_id=pr_order)
WHERE pr_order_accepted=false OR (o_order_status<4 OR (o_order_status>3 AND DATE(o_since)=CURRENT_DATE))
ORDER BY (pr_name,pr_order);
