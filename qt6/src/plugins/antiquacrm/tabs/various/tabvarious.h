// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUS_H
#define ANTIQUACRM_PLUGIN_VARIOUS_H

#include "variousconfig.h"
#include <AntiquaTabs>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_VARIOUS_PLUGIN TabVarious final : public AntiquaCRM::TabsInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_TABS_INTERFACE FILE "various.json")
  Q_INTERFACES(AntiquaCRM::TabsInterface)

public:
  explicit TabVarious(QObject* parent = nullptr);

  const QString displayName() const override;

  const QString interfaceName() const override;

  const QString sqlTableName() const override;

  const QString sqlFieldPrefix() const override;

  AntiquaCRM::PluginConfigWidget* configWidget(QWidget* parent) const override;

  bool addIndexOnInit() const override;

  const QJsonObject menuEntry() const override;

  AntiquaCRM::TabsIndex* indexWidget(QWidget* parent) const override;

  bool createInterface(QObject* parent) override;
};

#endif // ANTIQUACRM_PLUGIN_VARIOUS_H
