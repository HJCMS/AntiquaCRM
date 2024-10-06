-- @brief Company Info Settings
-- @file query_customer_overview.sql
SELECT c_provider_import AS importname,
  c_postal_address AS invoice,
  c_shipping_address AS delivery
FROM customers WHERE @SQL_WHERE_CLAUSE@;
