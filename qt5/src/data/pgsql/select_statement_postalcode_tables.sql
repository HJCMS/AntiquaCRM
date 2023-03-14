-- @brief Autofill Query for Postalcode Tables
-- @file select_statement_postalcode_tables.sql
SELECT json_build_object('plz', p_plz,'location', p_location,'state', p_state)
