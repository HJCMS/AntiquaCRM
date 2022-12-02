// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDER_H
#define ANTIQUACRM_PROVIDER_H

#include <AGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkCookie>
#include <QObject>
#include <QUrl>

class SqlPsql;
class Settings;
class Networker;

class Provider : public QObject {
  Q_OBJECT

protected:
  Settings *m_config;
  SqlPsql *m_sql;

  /**
   * @brief API access Remote URL
   */
  QUrl baseUrl;

  /**
   * @brief API access Path
   */
  QString apiPath;

  /**
   * @brief API access Key
   */
  QString apiKey;

  /**
   * @brief Actions Session Cookie
   */
  QNetworkCookie actionsCookie;

  /**
   * @brief Authentication Cookie
   */
  QNetworkCookie authenticCookie = QNetworkCookie();

  Networker *m_networker;

  /**
   * @brief uppercase first letter
   */
  const QString ucFirst(const QString &);

  /**
   * @brief Convert Gender from String to AntiquaCRM::Gender
   */
  AntiquaCRM::Gender convertGender(const QString &from) const;

  /**
   * @brief Search for IETF BCP 47 Language tag with Country name
   * @return "IETF BCP 47 language tag"
   */
  const QString bcp47Country(const QString &country) const;

  /**
   * @brief Search for Country name with IETF BCP 47 language tag
   */
  const QString getCountry(const QString &bcp47) const;

  /**
   * @brief Vendors respond - with different date/time and zone formats.
   */
  const QDateTime getDateTime(const QString &dateString,
                              const QString &timeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  const QDateTime getDateTime(const QString &dateTimeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  /**
   * @brief Convert Datetime to TimeSpec
   */
  const QDateTime timeSpecDate(const QDateTime &dateTime,
                               Qt::TimeSpec fromSpec = Qt::LocalTime) const;

  /**
   * @brief Convert Invalid Price formats to double
   */
  double getPrice(const QJsonValue &value) const;

  qint64 getArticleId(const QJsonValue &value) const;

  virtual void initConfiguration() = 0;

  virtual void prepareContent(const QJsonDocument &) = 0;

  virtual void database(const QList<QJsonObject> &) = 0;

protected Q_SLOTS:
  virtual void responsed(const QByteArray &) = 0;

Q_SIGNALS:
  void sendFinished();

public Q_SLOTS:
  virtual void queryOrders() = 0;

public:
  explicit Provider(QObject *parent = nullptr);
  virtual bool isAccessible() = 0;
};

#endif // ANTIQUACRM_PROVIDER_H
