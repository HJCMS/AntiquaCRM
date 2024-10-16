// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_ABEBOOKS_CONFIGWIDGET_H
#define ABEBOOKS_PLUGIN_ABEBOOKS_CONFIGWIDGET_H

#include "abebooksconfig.h"
#include <AntiquaProviders>
#include <AntiquaWidgets>
#include <QLabel>
#include <QUrl>
#include <QWidget>

class ABEBOOKS_PLUGIN AbeBooksConfigWidget final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::TextLine* m_api_user;
  AntiquaCRM::TextLine* m_api_host;
  AntiquaCRM::TextLine* m_api_key;
  AntiquaCRM::TextLine* m_seller_id;
  AntiquaCRM::NumEdit* m_api_port;
  inline QLabel* label(const QString& text);
  const QUrl apiUrl() const;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit AbeBooksConfigWidget(QWidget* parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ABEBOOKS_PLUGIN_ABEBOOKS_CONFIGWIDGET_H
