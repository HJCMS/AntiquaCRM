-- @brief Update Provider Treeview
-- @file query_provider_orders.sql
-- @see AntiquaCRM::OrderStatus
-- @note Offene oder stornierte Bestellungen 20 Tage lang anzeigen
SELECT
 pr_name AS order_provider,
 pr_order AS order_number,
 COALESCE(o_order_status,0) AS order_status,
 COALESCE(pr_created,CURRENT_TIMESTAMP) AS order_datetime,
 (CASE WHEN length(pr_buyer) > 20 THEN CONCAT(LEFT(pr_buyer, 20),'...') ELSE pr_buyer END) AS order_buyername,
 pr_buyer AS order_comment,
 pr_order_accepted, pr_hide
FROM provider_orders
LEFT JOIN inventory_orders ON (o_provider_name=pr_name AND o_provider_order_id=pr_order)
WHERE (pr_order_accepted=true AND o_order_status<4) OR
 (pr_created BETWEEN (CURRENT_TIMESTAMP - justify_interval(interval '20 days')) AND CURRENT_TIMESTAMP AND pr_order_accepted=false)
ORDER BY (pr_name,pr_order,pr_created);
