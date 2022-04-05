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

namespace HJCMS {

class SqlConfig;

class SqlCore : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QSqlDatabase p_db;
  SqlConfig *m_cfg;
  QSqlDatabase *database;
  int statusTimerID;
  /**
     @brief Fehler Meldungen aufarbeiten
     @return ERROR Message
   */
  const QString prepareErrorMessage();

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
     @brief Registrierten Verbindungsnamen abrufen.
     @return connectionName
   */
  const QString getConnectionName();

  virtual ~SqlCore();
};

};     // namespace HJCMS
#endif // HJCMS_SQLCORE_H
