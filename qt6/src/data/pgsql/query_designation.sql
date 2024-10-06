-- @brief Autofill Query Keywords
-- @file query_designation.sql
SELECT json_build_object('name',bd_keyword) FROM ref_book_designation ORDER BY bd_keyword ASC;
