// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGPATHS_H
#define ANTIQUACRM_DIALOGS_CONFIGPATHS_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class PathSettingsGroup;

class ConfigPaths final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  PathSettingsGroup *m_pathsGroup;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigPaths(QWidget *parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGPATHS_H
