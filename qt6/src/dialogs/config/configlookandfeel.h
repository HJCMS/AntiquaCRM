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
#include <QCheckBox>
#include <QGridLayout>
#include <QObject>

class LookAndFeelLayout final : public QGridLayout {
  Q_OBJECT

public:
  explicit LookAndFeelLayout(QWidget *parent = nullptr);
  void addToolTip(int row, int column, const QString &text);
};

class IconThemes;

class ConfigLookAndFeel final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

private:
  LookAndFeelLayout *layout;
  AntiquaCRM::BoolBox *m_toolTipIcons;
  AntiquaCRM::BoolBox *m_wheelSupport;
  IconThemes *m_iconThemes;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigLookAndFeel(QWidget *parent = nullptr);
  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGLOOKANDFEEL_H
