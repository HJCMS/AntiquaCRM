// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ABEBOOKS_H
#define ANTIQUACRM_ABEBOOKS_H

#include <QObject>
#include "provider.h"

class AbeBooks final : public Provider {
  Q_OBJECT

private:
  QString apiUser;
  QString apiPass;
  qint64 apiPort;

  void initConfiguration() override;

  void prepareContent(const QJsonDocument &) override {};

private Q_SLOTS:
  void responsed(const QByteArray &) override {};

public Q_SLOTS:
  void queryOrders() override;

public:
  explicit AbeBooks(QObject *parent = nullptr);
  bool isAccessible() override { return false; }
};

#endif
