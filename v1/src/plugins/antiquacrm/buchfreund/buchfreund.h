// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BUCHFREUND_LIBRARY_H
#define BUCHFREUND_LIBRARY_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QJsonValue>
#include <QMetaType>
#include <QObject>

class ANTIQUACRM_LIBRARY Buchfreund final
    : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "buchfreund.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  QString apiPath;

  const QHash<QString, QString> p_articleTranslate;
  bool initConfigurations();
  const QUrl apiQuery(const QString &section);
  const QString dateString(const QDate &date = QDate::currentDate()) const;
  const QVariant createValue(QMetaType::Type id, const QJsonValue &value) const;

  void setOrderItem(AntiquaCRM::AProviderOrder *order, const QString &key,
                    const QJsonValue &value) const;

  const AntiquaCRM::ArticleOrderItem
  setArticleItem(AntiquaCRM::AProviderOrder *order, const QString &key,
                 const QJsonValue &value) const;

private Q_SLOTS:
  void authenticate(){/* unused */};
  void prepareResponse(const QJsonDocument &js);
  void prepareResponse(const QDomDocument &){/* unused */};
  void queryFinished(QNetworkReply *reply);

public Q_SLOTS:
  void queryNewOrders(int waitSecs = 1);
  void queryOrder(const QString &orderId);
  void orderUpdateAction(const QJsonObject &options);

public:
  explicit Buchfreund(QObject *parent = nullptr);
  bool authenticationRequired();
  const QString configProvider() const;
  const QString displayName() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
