// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGDATABASE_H
#define ANTIQUACRM_DIALOGS_CONFIGDATABASE_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class ConfigDatabase final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

private:
    //

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigDatabase(QWidget *parent = nullptr);
  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGDATABASE_H
