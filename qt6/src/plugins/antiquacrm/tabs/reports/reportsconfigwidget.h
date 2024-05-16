// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_CONFIGWIDGET_H
#define ANTIQUACRM_REPORTS_CONFIGWIDGET_H

#include <AntiquaWidgets>

class ANTIQUACRM_LIBRARY ReportsConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ReportsConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_REPORTS_CONFIGWIDGET_H
