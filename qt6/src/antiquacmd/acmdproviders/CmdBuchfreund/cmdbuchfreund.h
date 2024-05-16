// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_CMDBUCHFREUND_H
#define ANTIQUACMD_CMDBUCHFREUND_H

#include <QByteArray>
#include <QJsonDocument>
#include <QObject>
#include <QStringList>

#ifndef ANTIQUACMD_PROVIDERS_H
#include "acmdproviders.h"
#endif

class CmdBuchfreund final : public ACmdProviders {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACMD_INTERFACE FILE "cmdbuchfreund.json")
  Q_INTERFACES(ACmdProviders)

private:
  void queryOrdersById(const QStringList &list);
  const QString provider() const override;
  void initConfiguration() override;
  const QUrl apiQuery(const QString &action) override;
  void prepareContent(const QJsonDocument &document) override;

private Q_SLOTS:
  void responsed(const QByteArray &bread) override;

public Q_SLOTS:
  void start() override;

public:
  explicit CmdBuchfreund(QObject *parent = nullptr);
  bool init() override;
};

#endif // ANTIQUACMD_CMDBUCHFREUND_H
