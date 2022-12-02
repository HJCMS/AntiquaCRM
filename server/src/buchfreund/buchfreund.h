// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BUCHFREUND_H
#define ANTIQUACRM_BUCHFREUND_H

#include <QObject>
#include "provider.h"

class Buchfreund final : public Provider {
  Q_OBJECT

private:
  void initConfiguration() override;

  void prepareContent(const QJsonDocument &) override {};

  void database(const QList<QJsonObject> &) override {};

private Q_SLOTS:
  void responsed(const QByteArray &) override {};

public Q_SLOTS:
  void queryOrders() override;

public:
  explicit Buchfreund(QObject *parent = nullptr);
  bool isAccessible() override { return false; }
};

#endif
