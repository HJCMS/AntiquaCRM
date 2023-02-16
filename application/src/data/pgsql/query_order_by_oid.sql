-- @brief Query Order with Order Id
-- @file query_order_by_oid.sql
SELECT o_id, o_order_status, o_payment_status, o_customer_id, o_invoice_id,
 o_delivery_service, o_delivery_send_id, o_delivery, o_provider_name,
 o_provider_order_id, o_provider_order, o_modified, o_since, o_delivery_package,
 o_delivered, o_vat_included, o_vat_levels, o_delivery_add_price,
 UPPER(GREATEST(TRIM(o_vat_country),TRIM(c_country_bcp47))) AS o_vat_country,
 o_payment_method, c_postal_address,
 (SELECT func_shipping_address(o_customer_id)) AS c_shipping_address
FROM inventory_orders
LEFT JOIN customers ON c_id=o_customer_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY o_id LIMIT 1;
