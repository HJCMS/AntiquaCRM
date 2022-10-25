// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_INTERFACE_H
#define ANTIQUACRM_PLUGIN_INTERFACE_H

#include <AntiquaCRM>
#include <QDateTime>
#include <QJsonDocument>
#include <QNetworkCookie>
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
  QUrl apiUrl;
  QString apiUser;
  QString apiKey;
  qint8 historyCall;
  QNetworkCookie actionsCookie;
  QNetworkCookie authenticCookie = QNetworkCookie();

  const QDateTime getDateTime(const QString &date, const QString &time,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  virtual void initConfigurations() = 0;

  virtual const QUrl apiQuery(const QString &section) = 0;

protected Q_SLOTS:
  virtual void prepareJsonResponse(const QJsonDocument &) = 0;
  virtual void queryFinished(QNetworkReply *) = 0;

Q_SIGNALS:
  void sendErrorResponse(AntiquaCRM::Message, const QString &);
  void sendQueryFinished();

public Q_SLOTS:
  virtual void queryOrders(int waitSecs = 1) = 0;

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
