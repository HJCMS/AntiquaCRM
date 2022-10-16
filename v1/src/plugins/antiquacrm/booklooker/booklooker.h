// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_LIBRARY_H
#define BOOKLOOKER_LIBRARY_H

#include <QObject>
#include <AntiquaCRMPlugin>

class ANTIQUACRM_LIBRARAY Booklooker final : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "booklooker.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

protected Q_SLOTS:
  void prepareJsonResponse(const QJsonDocument &);

public Q_SLOTS:
  void queryOrders(int waitSecs = 1);

public:
  explicit Booklooker(QObject *parent = nullptr);
  const QString configProvider() const;
  const QString displayName() const;
  const QJsonDocument getResponse() const;
  const AntiquaCRM::AProviderOrders getOrders() const;
  bool createInterface(QObject *parent);
};

#endif
