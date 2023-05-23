-- @brief Default loadDatabaseConfig
-- @file query_tab_config.sql
SELECT cfg_jsconfig FROM antiquacrm_configs WHERE @SQL_WHERE_CLAUSE@ LIMIT 1;
