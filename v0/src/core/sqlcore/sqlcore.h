// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCORE_SQLCORE_H
#define HJCMS_SQLCORE_SQLCORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QString>
#include <QStringList>
#include <QTimerEvent>

namespace HJCMS {

class SqlConfig;
class SqlConnection;

/**
 * @class HJCMS::SqlCore
 * @brief Stellt die Datenbankverbindung
 * Es wird mit @ref HJCMS::SqlConfig nach den Einstellungen
 * gesucht und eine Verbindung hergestellt.
 */
class SqlCore : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int queryTimeSpend = 0;
  QString p_sqlOptions;

  /**
   * @brief Test Verbindung
   * Wird erstellt bevor "database" Initialisiert wird!
   */
  QSqlDatabase p_db;

  /**
   * @brief Konfiguration auslesen
   */
  SqlConfig *m_cfg;

  /**
   * @brief Prüft die Socket Verbindung
   */
  SqlConnection *m_socket;

  /**
   * @brief Datenbank Verbindung
   */
  QSqlDatabase *database;

  /**
   * @brief Timer ID von startTimer
   */
  int statusTimerID;

  /**
   * @brief QSQLDriver inittialisieren
   */
  bool initSQLDriver();

  /**
   * @brief An Host:Port eine Verbindungsanfrage stellen.
   */
  bool socketConnectionTest();

  /**
   * @brief Datenbank Einstellungen Initieren
   * @note Es werden nur folgende Operationen vorgenommen.
   * @li Zugangs/Anmelde- Einstellungen.
   * @li Verbindungs Status auf Adresse:Port überprüfen.
   * @li Prüfung auf QSqlDatabase.isValid()
   * @note SQL Datenbank Verbindungsfehler werden in den QAbstractItem* Models
   * abgefangen.
   */
  bool initDatabase();

protected:
  /**
   * @brief TimerEvent für die TCP Socket Verbindung
   */
  void timerEvent(QTimerEvent *);

Q_SIGNALS:
  /**
   * @brief Operations Nachrichten
   */
  void s_noticeMessage(const QString &);

  /**
   * @brief SQLQuery Fehlermeldungen
   */
  void s_errorMessage(const QString &);

  /**
   * @brief Status zur TCP:Verbindung
   * @note Nicht die SQL Authentifizierung
   */
  void sendConnectionStatus(bool);

public Q_SLOTS:
  /**
   * @brief Die Datenbank Verbindung erneut herstellen
   * @param b (Unused)
   */
  void openDatabase(bool b = false);

public:
  explicit SqlCore(QObject *parent = nullptr);

  /**
   * @brief Nachsehen ob der SQL Treiber geladen wurde!
   * @return bool
   */
  bool sqlDriversExists();

  /**
   * @brief Fehler Meldungen aufarbeiten
   * @return Vollständige Fehler Meldung
   */
  const QString fetchErrors();

  /**
   * @brief Letzte Fehlermeldung ausgeben!
   * @return Nur Fehlermeldung
   */
  const QString lastError();

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
   * @brief Wie lange die letzte Abfrage gedauert hat.
   * Wird in Millisekunden zurück ausgegeben und nur bei exec() ausgeführt!
   */
  int timeSpend();

  /**
   * @brief Stelle eine SQL Abfrage
   * @param statement SQL-Statement
   */
  const QSqlQuery query(const QString &statement);

  /**
   * @brief Stelle ein Feldabfrage an Tabelle
   * @param Tabelle
   */
  const QSqlRecord record(const QString &table);

  /**
   * @brief Fremdschlüssel Constraint zu Tabelle auflösen
   * @param Von Tabelle
   * @param Mit Constraint Schlüsslename
   */
  const QString foreignTable(const QString &table, const QString &name);

  /**
   * @brief Alle Feldnamen von Tabelle
   * @param Tabelle
   */
  const QStringList fields(const QString &table);

  /**
   * @brief Datums Abfrage von bis ...
   * @param field - SQL Feldname
   * @param past  - tage in der Vergangenheit. Siehe QDate::addDays(int);
   * @return SQL-Clause
   */
  const QString whereDate(const QString &field, int past = -1) const;

  /**
   * @brief Schliest die Datenbanken
   * @note Darf nur beim Beenden der Anwendung ausgeführt werden!
   */
  void close();

  virtual ~SqlCore();
};

};     // namespace HJCMS
#endif // HJCMS_SQLCORE_SQLCORE_H
