// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGLOOKANDFEEL_H
#define ANTIQUACRM_DIALOGS_CONFIGLOOKANDFEEL_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class IconThemes;

class ConfigLookAndFeel final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::BoolBox *m_toolTipIcons;
  AntiquaCRM::BoolBox *m_wheelSupport;
  IconThemes *m_iconThemes;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigLookAndFeel(QWidget *parent = nullptr);
  AntiquaCRM::PluginConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGLOOKANDFEEL_H
