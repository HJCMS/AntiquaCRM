// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BUCHFREUND_CONFIGWIDGET_H
#define ANTIQUACRM_PLUGIN_BUCHFREUND_CONFIGWIDGET_H

#include <AntiquaProviders>
#include <AntiquaWidgets>
#include <QLabel>
#include <QUrl>
#include <QWidget>

class ANTIQUACRM_LIBRARY BuchfreundConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::TextLine *m_api_host;
  AntiquaCRM::TextLine *m_api_path;
  AntiquaCRM::TextLine *m_api_key;
  AntiquaCRM::NumEdit *m_api_port;
  inline QLabel *label(const QString &text);

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  /**
   * @param parent - parent object
   */
  explicit BuchfreundConfigWidget(QWidget *parent = nullptr);

  AntiquaCRM::ConfigType getType() const override;

  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_PLUGIN_BUCHFREUND_CONFIGWIDGET_H
