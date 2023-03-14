-- @brief Autofill Query for Book Bindings
-- @file query_book_binding.sql
SELECT json_build_object('id',bb_id,'name',bb_name,'type',bb_type,'description',bb_description)
FROM ref_book_binding WHERE bb_id>0 ORDER BY bb_type,bb_id ASC;
