// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_LIBRARY_H
#define BOOKLOOKER_LIBRARY_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QObject>

class ANTIQUACRM_LIBRARAY Booklooker final
    : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "booklooker.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  /**
   * @brief read configuration
   */
  void initConfigurations();

  /**
   * @brief Standard Anfrage Url erstellen
   * @param section  - Abfrageteil im Url Pfad
   * @return Rest API URL
   */
  const QUrl apiQuery(const QString &section);

  void setTokenCookie(const QString &token);
  bool isCookieExpired();

private Q_SLOTS:
  void authenticate();
  void queryFinished(QNetworkReply *);

protected Q_SLOTS:
  void prepareJsonResponse(const QJsonDocument &);

public Q_SLOTS:
  void queryOrders(int waitSecs = 1);

public:
  explicit Booklooker(QObject *parent = nullptr);
  const QString configProvider() const;
  const QString displayName() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
