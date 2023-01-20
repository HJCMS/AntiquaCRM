-- @short Datenbank Status abfragen
-- @file query_connection_info.sql
SELECT
usename AS benutzer,
application_name AS anwendung,
client_addr::text AS client,
state AS status,
(CASE
  WHEN b.ssl=true THEN 'Gesicherte Verbindung mit SSL/TLS'
  WHEN c.encrypted THEN 'Gesichert Verbindung mit Kerberos'
  ELSE 'Warnung ungesicherte Verbindung'
 END)::text AS Verbindung
FROM pg_stat_activity AS a
LEFT JOIN pg_stat_ssl AS b ON b.pid=a.pid
LEFT JOIN pg_stat_gssapi AS c ON c.pid=a.pid
WHERE state = 'active';
