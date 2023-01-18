# Antiqua CRM
Ist ein Programm zur Verwaltung von Antiquarischen Büchern.

Es bietet eine Verwaltung der Artikelbestände und einige Online Schnittstellen für eine vereinfachte Datenverwaltung zu den einzelnen Dienstanbietern.

Das Programm wird mit dem Qt-C++ Framework ab Version 5.12.\* *Community Lizens* geschrieben und verwendet eine PostgreSQL Datenbank ab Version 14.* für die Datenverwaltung.

#### Zur Zeit sind folgende Funktionen enthalten:
- Klient Anwendung:
  - Kunden Adressbuch,
  - Buch, Drucke+Stiche und CD+Vinyl Verwaltung,
  - Auftrags/Bestellverwaltung *(Ist abhängig von der Serveranwendung!)*,
  - Remote Office über SSL/TLS,
  - Bilder Verwaltung *(Über CIFS/SMB Schnittstelle >= 2.\* im lokalen Netzwerk)*,
  - Statistiken.

> Die Klientanwendung stellt keine Remoteanfragen zu den Dienstleistern!<br>Dies macht die Serveranwendung mittels eines Intervals und fügt die Bestellungen in die Datenbank ein.

- Server Anwendung:
  - Dienstleisterabfrage der Bestellungen und einfügen in das Auftragssystem.
  - CronJob-Scripte: Zum täglichen Hochladen der Bilder und Update der Datenbestände.

---

![Demo](preview.png)

---

### Diese Projekte werden eingebunden:
1. [Qt Framework] https://www.qt.io/product/framework "Qt Framework"
2. [PostgreSQL] https://www.postgresql.org "PostgreSQL"
3. [libqrencode] https://github.com/fukuchi/libqrencode "QR Code encoding library"

