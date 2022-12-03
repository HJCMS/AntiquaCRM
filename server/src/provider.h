// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDER_H
#define ANTIQUACRM_PROVIDER_H

#include <AGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QNetworkCookie>
#include <QObject>
#include <QUrl>

class SqlPsql;
class Settings;
class Networker;

class Provider : public QObject {
  Q_OBJECT

private:
  SqlPsql *m_sql;

protected:
  Settings *m_config;
  Networker *m_networker;

  QUrl baseUrl;

  QString apiPath;

  QString apiKey;

  QNetworkCookie actionsCookie;

  QNetworkCookie authenticCookie = QNetworkCookie();

  virtual void initConfiguration() = 0;

  virtual void prepareContent(const QJsonDocument &) = 0;

  const QString ucFirst(const QString &);

  AntiquaCRM::Gender convertGender(const QString &from) const;

  /**
   * @brief Find IETF BCP 47 language tag
   * @param country
   */
  const QString findBCP47(const QString &country) const;

  const QString getCountry(const QString &bcp47) const;

  const QDateTime getDateTime(const QString &dateString,
                              const QString &timeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  const QDateTime getDateTime(const QString &dateTimeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  const QDateTime timeSpecDate(const QDateTime &dateTime,
                               Qt::TimeSpec fromSpec = Qt::LocalTime) const;

  double getPrice(const QJsonValue &) const;

  qint64 convertArticleId(const QJsonValue &) const;

  const QStringList currentOrderIds(const QString &provider);

  bool createOrders(const QList<QJsonObject> &);

  QPair<qint64, QString> findInsertCustomer(const QJsonObject &);

protected Q_SLOTS:
  virtual void responsed(const QByteArray &) = 0;

Q_SIGNALS:
  void sendDisjointed();
  void sendFinished();

public Q_SLOTS:
  virtual void queryOrders() = 0;

public:
  explicit Provider(QObject *parent = nullptr);
  virtual bool isAccessible() = 0;
};

#endif // ANTIQUACRM_PROVIDER_H
