-- @brief Query PostgreSQL Views
-- @file query_views.sql
SELECT table_name,obj_description(table_name::regclass) AS comment
FROM information_schema.tables
WHERE table_schema = current_schema AND table_type='VIEW';
