-- @brief Used to create missing payments notification of the day
-- @file query_order_notifiaction.sql
SELECT o_id, o_provider_name, CONCAT(c_firstname, ' ', c_lastname) AS name, o_since AS since
FROM inventory_orders
LEFT JOIN customers ON c_id=o_customer_id
LEFT JOIN ref_delivery_service ON d_id=o_delivery_service
WHERE (o_order_status!=6 AND o_payment_status NOT IN (1,4)) OR (o_order_status NOT IN (4,6) AND o_payment_status NOT IN (1,4)) AND o_since BETWEEN (CURRENT_TIMESTAMP - justify_interval(interval '12 months')) AND CURRENT_TIMESTAMP
ORDER BY o_id DESC;
