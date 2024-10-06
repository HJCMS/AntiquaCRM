-- @brief Load storage cache configuration
-- @file query_antiquacrm_cacheconf.sql
SELECT * FROM antiquacrm_cacheconf WHERE cache_table IS NOT NULL;
