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

  const QUrl apiQuery(const QString &) override;

  const QString provider() const override { return QString("Buchfreund"); };

  void queryOrdersById(const QStringList &);

  void prepareContent(const QJsonDocument &) override;

private Q_SLOTS:
  void responsed(const QByteArray &) override;

public Q_SLOTS:
  void start() override;

public:
  explicit Buchfreund(QObject *parent = nullptr);
  bool init() override;
};

#endif
