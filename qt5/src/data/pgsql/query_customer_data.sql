-- @brief Query Customer
-- @file query_customer_data.sql
SELECT *, CONCAT_WS(' ', c_title, c_firstname, c_lastname) AS c_fullname
FROM customers WHERE @SQL_WHERE_CLAUSE@ ORDER BY c_id;

