-- @short Datenbank Status abfragen
-- @file query_connection_info.sql
SELECT usename, application_name, client_addr::text AS client_addr, state AS status,
(CASE WHEN b.ssl=true THEN 'DB_TLS_SECURED'
  WHEN c.encrypted THEN 'DB_KRB_SECURED' ELSE 'DB_UNSECURED' END)::text AS connection
FROM pg_stat_activity AS a
LEFT JOIN pg_stat_ssl AS b ON b.pid=a.pid
LEFT JOIN pg_stat_gssapi AS c ON c.pid=a.pid
WHERE state = 'active';
