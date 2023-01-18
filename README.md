# Antiqua CRM
Ist ein Programm zur Verwaltung von Antiquarischen Büchern.

Es bietet eine Verwaltung der Artikelbestände und einige Online Schnittstellen für eine vereinfachte Datenverwaltung zu einzelnen Dienstanbietern.

Das Programm wird mit der Qt-C++ Version 5.12.* Community Lizens geschrieben und verwendet eine PostgreSQL Datenbank ab Version 14.* für die Datenverwaltung.

#### Zur Zeit sind folgende Funktionen enthalten:
- *Klient Anwendung*:
  - Kunden Adressbuch
  - Buch, Drucke+Stiche und CD+Vinyl Verwaltung
  - Auftrags/Bestellverwaltung (Ist anhängig von der Serveranwendung!)
  - Remote Office über SSL/TLS
  - Bilder Verwaltung (Nur im lokalen Netzwerk)
  - Statistiken

- *Server Anwendung*:
  - Dienstleister abfrage der Bestellungen und einfügen in das Auftragssystem.
  - Cron Jobs - Zum täglichen Hochladen der Bilder und Daten.

---

![Demo](preview.png)

