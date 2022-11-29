// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_LIBRARY_H
#define ABEBOOKS_LIBRARY_H

#include <AntiquaCRMPlugin>
#include <QDomElement>
#include <QObject>

class AbeBooksDocument;

class ANTIQUACRM_LIBRARY Abebooks final : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "abebooks.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  bool initConfigurations();
  AbeBooksDocument initDocument();
  const QUrl apiQuery(const QString &section);
  const QString dateString(const QDate &date = QDate::currentDate()) const;

  const QVariant createValue(QMetaType::Type id, const QJsonValue &value) const;

  void setOrderItem(AntiquaCRM::AProviderOrder *order, const QString &key,
                    const QJsonValue &value) const;

  const AntiquaCRM::ArticleOrderItem setArticleItem(AntiquaCRM::AProviderOrder *order, const QString &key,
                      const QJsonValue &value) const;

private Q_SLOTS:
  void authenticate(){/* unused */};
  void prepareResponse(const QJsonDocument &js);
  void prepareResponse(const QDomDocument &xml);
  void queryFinished(QNetworkReply *reply);

public Q_SLOTS:
  void queryNewOrders();
  void queryOrder(const QString &orderId);
  void orderUpdateAction(const QJsonObject &options);

public:
  explicit Abebooks(QObject *parent = nullptr);
  bool authenticationRequired() { return false; };
  const QString configProvider() const;
  const QString displayName() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
