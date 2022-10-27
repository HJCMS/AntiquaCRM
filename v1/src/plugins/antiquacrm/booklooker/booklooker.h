// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_LIBRARY_H
#define BOOKLOOKER_LIBRARY_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QJsonValue>
#include <QObject>

class ANTIQUACRM_LIBRARAY Booklooker final
    : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "booklooker.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  const QHash<QString, QString> p_articleTranslate;

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

  const QString dateString(const QDate &date = QDate::currentDate()) const;

  const AntiquaCRM::AProviderOrderItem articleItem(const QString &key,
                                                   const QJsonValue &value) const;

  void setTokenCookie(const QString &token);
  bool isCookieExpired();

private Q_SLOTS:
  void authenticate();
  void prepareResponse(const QJsonDocument &js);
  void prepareResponse(const QDomDocument &xml);
  void queryFinished(QNetworkReply *reply);

public Q_SLOTS:
  void queryNewOrders(int waitSecs = 1);
  void queryOrder(const QString &orderId);

public:
  explicit Booklooker(QObject *parent = nullptr);
  const QString configProvider() const;
  const QString displayName() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
