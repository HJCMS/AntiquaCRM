-- @brief Query Replace Order Customer Data
-- @file query_customer_new_order.sql
SELECT c_id AS o_customer_id,
  c_postal_address,
  c_shipping_address,
  d_srv AS o_delivery_service,
  d_cid AS o_delivery_package,
  c_country_bcp47 AS o_vat_country,
  'Office' AS o_provider_name,
  to_char(current_timestamp, 'YYYY-MM-DD') AS o_provider_order_id
FROM customers
LEFT JOIN ref_delivery_cost ON d_default=true
WHERE @SQL_WHERE_CLAUSE@;
