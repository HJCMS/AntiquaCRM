// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BUCHFREUND_H
#define ANTIQUACRM_PLUGIN_BUCHFREUND_H

#include "buchfreundconfig.h"
#include <AntiquaProviders>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

class BUCHFREUND_PLUGIN Buchfreund final : public AntiquaCRM::ProviderInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_PROVIDER_INTERFACE FILE "buchfreund.json")
  Q_INTERFACES(AntiquaCRM::ProviderInterface)

public:
  explicit Buchfreund(QObject* parent = nullptr);
  virtual const QString displayName() const override;
  virtual const QString interfaceName() const override;
  virtual AntiquaCRM::PluginConfigWidget* configWidget(QWidget* parent) const override;
  virtual bool operationWidget(QWidget* parent, const QJsonObject& order) const override;
  virtual const QJsonObject menuEntry() const override;
  virtual bool createInterface(QObject* parent) override;
};

#endif // ANTIQUACRM_PLUGIN_BUCHFREUND_H
