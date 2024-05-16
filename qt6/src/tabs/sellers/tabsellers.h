// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_TABSELLERS_H
#define ANTIQUACRM_PLUGIN_SELLERS_TABSELLERS_H

#include <AntiquaTabs>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY TabSellers final : public AntiquaCRM::TabsInterface {
  Q_OBJECT
  Q_INTERFACES(AntiquaCRM::TabsInterface)

public:
  explicit TabSellers(QObject *parent = nullptr);

  const QString displayName() const override;

  const QString interfaceName() const override;

  const QString sqlTableName() const override;

  const QString sqlFieldPrefix() const override;

  AntiquaCRM::PluginConfigWidget *configWidget(QWidget *parent) const override;

  bool addIndexOnInit() const override;

  const QJsonObject menuEntry() const override;

  AntiquaCRM::TabsIndex *indexWidget(QWidget *parent) const override;

  bool createInterface(QObject *parent) override;
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_TABSELLERS_H
