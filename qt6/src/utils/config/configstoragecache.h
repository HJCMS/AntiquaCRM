// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGSTORAGECACHE_H
#define ANTIQUACRM_DIALOGS_CONFIGSTORAGECACHE_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

class ConfigStorageCache final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  QHeaderView *m_header;
  QTableWidget *m_table;

private Q_SLOTS:
  void columnChanged(QTableWidgetItem *);

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigStorageCache(QWidget *parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGSTORAGECACHE_H
