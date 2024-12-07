-- @brief Load storage cache configuration
-- @file query_antiquacrm_cacheconf.sql
SELECT * FROM antiquacrm_cacheconf WHERE cache_type='t' AND cache_table IS NOT NULL ORDER BY cache_timestamp ASC;
