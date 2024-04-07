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

namespace AntiquaCRM
{

class ASqlSettings;

/**
 * @brief Primary class for the database connection
 * @ingroup CoreLibrary
 *
 * By default, AntiquaCRM::ASqlCore checks the drivers and accesses the database
 * connection!
 *
 * @note AntiquaCRM opens a session connection when it starts!
 */
class ANTIQUACRM_LIBRARY ASqlCore final : public QObject {
  Q_OBJECT

private:
  ASqlSettings* config;
  QSqlDatabase* database;

  /**
   * @brief manual defined Network Connection error!
   */
  const QSqlError sqlNetworkError() const;

  /**
   * @brief create database connection
   */
  bool initDatabase();

  /**
   * @brief check connection
   */
  bool isConnected();

private Q_SLOTS:
  void prepareSqlError(const QSqlError&);

Q_SIGNALS:
  void sendStatementError(const QSqlError&);

public:
  /**
   * @param parent - parent object
   * @param profile - force to use a custom profile
   *
   * Optional: Assign a profile for the database configuration.
   * By default, this will read in AntiquaCRM::ASqlSettings class and read from
   * user configuration in the UI.
   * e.g.: When using the server, this can be overwritten here.
   */
  explicit ASqlCore(QObject* parent = nullptr, const QString& profile = QString());

  /**
   * @brief network connection test
   *
   * This function searches for available network interfaces.
   */
  static bool networkStatus();

  /**
   * @brief Responses the max database query limit for tables.
   */
  qint64 getQueryLimit();

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
   * @brief Query CURRENT_TIMESTAMP from SQL-Server.
   * @note timezone depends on server locale
   */
  const QString getTimeStamp();

  /**
   * @brief Query DateTime from SQL-Server.
   * @note timezone depends on server locale
   */
  const QDateTime getDateTimeStamp();

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
  const QSqlRecord record(const QString& table);

  /**
   * @brief Take field names from table
   */
  const QStringList fieldNames(const QString& table);

  /**
   * @brief Standard processing of SQL queries.
   */
  const QSqlQuery query(const QString& statement);

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
