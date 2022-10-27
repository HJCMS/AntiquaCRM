// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_INTERFACE_H
#define ANTIQUACRM_PLUGIN_INTERFACE_H

#include <AntiquaCRM>
#include <QDate>
#include <QDateTime>
#include <QDomDocument>
#include <QJsonDocument>
#include <QNetworkCookie>
#include <QTextCodec>
#include <QObject>
#include <QString>
#include <QUrl>

#include "aproviderorders.h"

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARAY APluginInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM Plugin Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

protected:
  AntiquaCRM::ANetworker *m_network;
  QTextCodec *m_decodeFrom;

  /**
   * @brief API access Remote URL
   */
  QUrl apiUrl;

  /**
   * @brief API access Loginname
   */
  QString apiUser;

  /**
   * @brief API access Key
   */
  QString apiKey;

  /**
   * @brief Some providers needs a history request
   */
  qint8 historyCall;

  /**
   * @brief Actions Session Cookie
   */
  QNetworkCookie actionsCookie;

  /**
   * @brief Authentication Cookie
   */
  QNetworkCookie authenticCookie = QNetworkCookie();

  /**
   * @brief Vendors respond with different date and time formats.
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
   * @brief Vendors using different Date time formats!
   */
  virtual const QString
  dateString(const QDate &date = QDate::currentDate()) const = 0;

  /**
   * @brief load API access configuration
   */
  virtual void initConfigurations() = 0;

  /**
   * @brief create a custom API access
   */
  virtual const QUrl apiQuery(const QString &section) = 0;

protected Q_SLOTS:
  void setContentDecoder(QTextCodec *);
  virtual void prepareResponse(const QJsonDocument &js) = 0;
  virtual void prepareResponse(const QDomDocument &xml) = 0;
  virtual void queryFinished(QNetworkReply *reply) = 0;

Q_SIGNALS:
  void sendErrorResponse(AntiquaCRM::Message, const QString &);
  void sendQueryFinished();

public Q_SLOTS:
  virtual void queryNewOrders(int waitSecs = 1) = 0;
  virtual void queryOrder(const QString &orderId) = 0;

public:
  explicit APluginInterface(QObject *parent = nullptr);

  const AntiquaCRM::APluginConfig getConfig(const QString &providerName);

  virtual const QString configProvider() const = 0;

  virtual const QString displayName() const = 0;

  virtual const AntiquaCRM::AProviderOrders getOrders() const = 0;

  virtual bool createInterface(QObject *parent) = 0;
};

}; // namespace AntiquaCRM

Q_DECLARE_INTERFACE(AntiquaCRM::APluginInterface, ANTIQUACRM_INTERFACE)

#endif // ANTIQUACRM_PLUGIN_INTERFACE_H
