-- @brief Query Customer
-- @file query_customer_exists.sql
SELECT c_id, c_trusted, CONCAT_WS(' ', c_title, c_firstname, c_lastname) AS display_name
FROM customers WHERE @SQL_WHERE_CLAUSE@ ORDER BY c_id;
