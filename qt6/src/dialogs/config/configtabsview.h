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

class ConfigTabsView final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

private:
  PluginListWidget *m_view;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigTabsView(QWidget *parent = nullptr);

  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;

  /**
   * @brief List of all viewable Tabs.
   */
  QList<AntiquaCRM::TabsInterface *> viewableTabs();

  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGTABSVIEW_H
