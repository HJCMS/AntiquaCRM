-- @brief Update Provider Treeview
-- @file query_provider_order_history.sql
-- @see AntiquaCRM::OrderStatus
SELECT pr_name, pr_order, pr_datetime, pr_buyer,
  (CASE WHEN o_order_status IS NULL THEN pr_status ELSE o_order_status END) AS pr_status
FROM provider_order_history
LEFT JOIN inventory_orders ON (o_provider_name=pr_name AND o_provider_order_id=pr_order)
WHERE (pr_status NOT IN (5,6,7) OR o_order_status NOT IN (5,6,7)) ORDER BY pr_name;
