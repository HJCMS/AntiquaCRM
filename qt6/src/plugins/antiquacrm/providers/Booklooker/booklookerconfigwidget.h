// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_CONFIGWIDGET_H
#define BOOKLOOKER_PLUGIN_CONFIGWIDGET_H

#include <AntiquaProviders>
#include <AntiquaWidgets>
#include <QUrl>
#include <QLabel>
#include <QWidget>

/**
 * @class BookLookerConfigWidget
 * @brief BookLooker Provider plugin Configuration
 *
 * @ingroup BookLooker
 */
class ANTIQUACRM_LIBRARY BookLookerConfigWidget final
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
  explicit BookLookerConfigWidget(QWidget *parent = nullptr);

  AntiquaCRM::ConfigType getType() const override;

  const QJsonObject getMenuEntry() const override;
};

#endif // BOOKLOOKER_PLUGIN_CONFIGWIDGET_H
