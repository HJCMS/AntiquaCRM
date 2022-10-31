// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_LIBRARY_H
#define ABEBOOKS_LIBRARY_H

#include <AntiquaCRMPlugin>
#include <QDomElement>
#include <QObject>

class AbeBooksDocument;

class ANTIQUACRM_LIBRARAY Abebooks final : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "abebooks.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  bool initConfigurations();
  AbeBooksDocument initDocument();
  const QUrl apiQuery(const QString &section);
  const QString dateString(const QDate &date = QDate::currentDate()) const;

private Q_SLOTS:
  void prepareResponse(const QJsonDocument &js);
  void prepareResponse(const QDomDocument &xml);
  void queryFinished(QNetworkReply *reply);

public Q_SLOTS:
  void queryNewOrders(int waitSecs = 1);
  void queryOrder(const QString &orderId);

public:
  explicit Abebooks(QObject *parent = nullptr);
  const QString configProvider() const;
  const QString displayName() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
