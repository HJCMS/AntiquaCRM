// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKLOOKER_CONFIGWIDGET_H
#define ANTIQUACRM_PLUGIN_BOOKLOOKER_CONFIGWIDGET_H

#include <AntiquaProviders>
#include <AntiquaWidgets>
#include <QUrl>
#include <QLabel>
#include <QWidget>

class ANTIQUACRM_LIBRARY BookLookerConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::TextLine *m_api_host;
  AntiquaCRM::TextLine *m_api_key;
  AntiquaCRM::NumEdit *m_api_port;
  inline QLabel *label(const QString &text);
  const QUrl apiUrl() const;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit BookLookerConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::PluginConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKLOOKER_CONFIGWIDGET_H
