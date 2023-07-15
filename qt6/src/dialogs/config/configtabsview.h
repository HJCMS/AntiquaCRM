// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGTABSVIEW_H
#define ANTIQUACRM_DIALOGS_CONFIGTABSVIEW_H

#include <AntiquaTabs>
#include <QList>
#include <QWidget>

class PluginListWidget;

class ConfigTabsView final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  PluginListWidget *m_view;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigTabsView(QWidget *parent = nullptr);

  AntiquaCRM::ConfigType getType() const override;

  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGTABSVIEW_H
