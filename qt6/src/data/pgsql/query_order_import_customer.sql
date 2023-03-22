-- @brief Query Replace Order Customer Data
-- @file query_order_import_customer.sql
SELECT c_country, c_email_0, c_firstname, c_gender, c_lastname,
 c_location, c_postal_address, c_postalcode, c_provider_import, c_street
FROM customers WHERE @SQL_WHERE_CLAUSE@;
