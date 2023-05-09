-- @brief Query Customer with Current OrderId
-- @file query_customer_recipient_address.sql
SELECT c_id AS cid, c_gender AS gender,
  TRIM(CONCAT(c_title, '', c_firstname, ' ', c_lastname)) AS person,
  CASE WHEN c_shipping_address IS NULL THEN c_postal_address ELSE c_shipping_address END AS address
FROM customers WHERE @SQL_WHERE_CLAUSE@ ORDER BY c_id;
