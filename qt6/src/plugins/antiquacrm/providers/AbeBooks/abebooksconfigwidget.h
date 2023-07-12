// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ABEBOOKS_CONFIGWIDGET_H
#define ANTIQUACRM_PLUGIN_ABEBOOKS_CONFIGWIDGET_H

#include <AntiquaProviders>
#include <QWidget>

class ANTIQUACRM_LIBRARY AbeBooksConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit AbeBooksConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::PluginConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_PLUGIN_ABEBOOKS_CONFIGWIDGET_H
