-- @brief Autofill Book Publishers
-- @file query_book_publisher_de.sql
SELECT json_build_object('name',p_name,'location',p_location) FROM ref_book_publisher_de ORDER BY p_name ASC;
