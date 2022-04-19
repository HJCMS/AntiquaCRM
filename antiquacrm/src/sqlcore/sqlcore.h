// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCORE_H
#define HJCMS_SQLCORE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTimerEvent>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace HJCMS {

class SqlConfig;

/**
 @class HJCMS::SqlCore
 @brief Stellt die Datenbankverbindung
 Es wird mit @ref HJCMS::SqlConfig nach den Einstellungen
 gesucht und eine Verbindung hergestellt.

*/
class SqlCore : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
    @brief Test Verbindung
    Wird erstellt bevor @ref database
    Initialisiert wird!
  */
  QSqlDatabase p_db;

  /**
    @brief Konfiguration auslesen
  */
  SqlConfig *m_cfg;

  /**
   @brief Datenbank Verbindung
  */
  QSqlDatabase *database;

  /**
     @brief Timer ID von startTimer
   */
  int statusTimerID;

  /**
     @brief QSQLDriver inittialisieren
     @return bool
   */
  bool initSQLDriver();

  /**
     @brief An Host:Port eine Verbindungsanfrage stellen.
     @todo Aktuell noch keine SSL Unterstützung!
     @return bool
   */
  bool socketConnectionTest();

  /**
     @brief Datenbank Einstellungen Initieren
     @note Es werden nur folgende Operationen vorgenommen.
        @li Zugangs/Anmelde- Einstellungen.
        @li Verbindungs Status auf Adresse:Port überprüfen.
        @li Prüfung auf QSqlDatabase.isValid()
     @note SQL Datenbank Verbindungsfehler werden in den
           QAbstractItem* Models abgefangen.
     @return bool
   */
  bool initDatabase();

protected:
  /**
     @brief TimerEvent für die TCP Socket Verbindung
   */
  void timerEvent(QTimerEvent *);

Q_SIGNALS:
  /**
     @brief Operations Nachrichten
   */
  void s_databaseMessage(const QString &);

  /**
     @brief SQLQuery Fehlermeldungen
  */
  void s_errorMessage(const QString &);

  /**
     @brief Status zur TCP:Verbindung
     @note Nicht die SQL Authentifizierung
  */
  void s_connectionStatus(bool);

public Q_SLOTS:
  /**
     @brief Fehler Meldungen aufarbeiten
     @return ERROR Message
   */
  const QString fetchErrors();

  /**
     @brief Die Datenbank Verbindung erneut herstellen
     @param b (Unused)
   */
  void openDatabase(bool b = false);

public:
  explicit SqlCore(QObject *parent = nullptr);

  /**
     @brief Nachsehen ob der SQL Treiber geladen wurde!
     @return bool
   */
  bool sqlDriversExists();

  /**
     @brief initialDatabase
     Wird bei der ersten Verbindung aufgerufen!
     @warning Ist nur einmal Notwendig!
   */
  bool initialDatabase();

  /**
     @brief connection
     @return Aktuelle Datenbank Verbindung
   */
  const QSqlDatabase db();

  /**
     @brief Registrierten Verbindungsnamen abrufen.
     @return connectionName
   */
  const QString getConnectionName();

  /**
     @brief Stelle eine SQL Abfrage
     @param statement SQL-Statement
     @return QSqlQuery
   */
  const QSqlQuery query(const QString &statement);

  /**
     @brief Stelle eine Feldabfrage
     @param Tabelle
     @return QSqlRecord
   */
  const QSqlRecord record(const QString &table);

  virtual ~SqlCore();
};

};     // namespace HJCMS
#endif // HJCMS_SQLCORE_H
