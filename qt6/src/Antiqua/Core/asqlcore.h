// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SQLCORE_H
#define ANTIQUACRM_SQLCORE_H

#include <AGlobal>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QtSql>

namespace AntiquaCRM {

class ASqlSettings;

/**
 * @brief Primärklasse für die Datenbank
 * @ingroup CoreLibrary
 * Die ASqlCore prüft Standardmäßig die Treiber und greift auf die
 * Datenbankverbindung zu!
 * @note AntiquaCRM macht beim Start eine Session-Verbindung auf!
 */
class ANTIQUACRM_LIBRARY ASqlCore final : public QObject {
  Q_OBJECT

private:
  static QMutex s_mutex;
  ASqlSettings *config;
  QSqlDatabase *database;

  /**
   * @brief Erstelle Datenbankverbindung
   */
  bool initDatabase();

  /**
   * @brief Prüfe Verbindung
   */
  bool isConnected();

private Q_SLOTS:
  void prepareSqlError(const QSqlError &);

Q_SIGNALS:
  void sendConnectionError(const QSqlError &);
  void sendStatementError(const QSqlError &);

public:
  /**
   * @param parent - parent object
   */
  explicit ASqlCore(QObject *parent = nullptr);

  /**
   * @brief Connection ApplicationName
   *
   * This Idientifier must uniq!
   */
  static const QString identifier();

  /**
   * @brief Locale date time string
   * @note not current timestamp!
   *
   * Current date time, as reported by the client-system clock.
   *
   * @code
   *    dd.MM.yyyy hh:mm.dd.ssss t
   * @endcode
   */
  const QString getDateTime() const;

  /**
   * @brief Query CURRENT_TIMESTAMP from PostgreSQL-Server.
   * @note it depends on server system locale
   */
  const QString getTimeStamp();

  /**
   * @brief Open database
   * @note Only needs to be called once.
   *
   * Usually called in the client application class.
   */
  bool open();

  /**
   * @brief Returns the status of the database connection!
   */
  bool status();

  /**
   * @brief Optionally get the current database connection!
   * @note Not recommended and only use it if there is no other option!
   */
  const QSqlDatabase db();

  /**
   * @brief Refer to detailed tables information.
   * @note It is better to use QSqlQuery::record() for nested queries!
   */
  const QSqlRecord record(const QString &table);

  /**
   * @brief Take field names from table
   */
  const QStringList fieldNames(const QString &table);

  /**
   * @brief Standard processing of SQL queries.
   */
  const QSqlQuery query(const QString &statement);

  /**
   * @brief Returns the last error message.
   */
  const QString lastError();

  /**
   * @brief Close database connection.
   *
   * Normally not necessary and only used in client application on quit!
   */
  void close();
};

}; // namespace AntiquaCRM

#endif // #ifndef ANTIQUACRM_SQLCORE_H
