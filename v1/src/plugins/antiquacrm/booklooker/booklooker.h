// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_LIBRARY_H
#define BOOKLOOKER_LIBRARY_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QJsonValue>
#include <QObject>

class ANTIQUACRM_LIBRARY Booklooker final
    : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "booklooker.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  bool initConfigurations() override;
  const QUrl apiQuery(const QString &section) override;
  const QString dateString(const QDate &date = QDate::currentDate()) const override;

  void setTokenCookie(const QString &token);

  bool isCookieExpired();

private Q_SLOTS:
  void authenticate() override;
  void prepareResponse(const QJsonDocument &js) override;
  void prepareResponse(const QDomDocument &xml) override;
  void queryFinished(QNetworkReply *reply) override;

public Q_SLOTS:
  void orderUpdateAction(const QJsonObject &options) override;

public:
  explicit Booklooker(QObject *parent = nullptr);
  bool authenticationRequired() override;
  const QString configProvider() const override;
  const QString displayName() const override;
  bool createInterface(QObject *parent) override;
};

#endif
