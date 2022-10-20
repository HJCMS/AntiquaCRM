-- @brief Autofill Query Keywords
-- @file query_designation.sql
SELECT json_build_object('name',k_keyword)
FROM ui_autofill_keywords WHERE k_table_cell='designation' ORDER BY k_keyword ASC;
