// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGPROVIDERSVIEW_H
#define ANTIQUACRM_DIALOGS_CONFIGPROVIDERSVIEW_H

#include <AntiquaTabs>
#include <QWidget>

class ConfigProvidersView final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

private:
  // QStringList p_list;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigProvidersView(QWidget *parent = nullptr);
  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGPROVIDERSVIEW_H
