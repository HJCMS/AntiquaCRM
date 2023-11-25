-- @brief Load json configuration from antiquacrm_configs
-- @file query_antiquacrm_config.sql
SELECT DISTINCT cfg_jsconfig FROM antiquacrm_configs WHERE @SQL_WHERE_CLAUSE@;
