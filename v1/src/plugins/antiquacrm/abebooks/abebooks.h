// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_LIBRARY_H
#define ABEBOOKS_LIBRARY_H

#include <QObject>
#include <AntiquaCRMPlugin>

class ANTIQUACRM_LIBRARAY Abebooks final : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "abebooks.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

protected:
  void initConfigurations();
  const QUrl apiQuery(const QString &section);

protected Q_SLOTS:
  void prepareJsonResponse(const QJsonDocument &);
  void queryFinished(QNetworkReply *);

public Q_SLOTS:
  void queryOrders(int waitSecs = 1);

public:
  explicit Abebooks(QObject *parent = nullptr);
  const QString configProvider() const;
  const QString displayName() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
