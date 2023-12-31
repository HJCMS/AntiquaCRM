// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKS_CONFIGWIDGET_H
#define ANTIQUACRM_PLUGIN_BOOKS_CONFIGWIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QJsonObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY BooksTabConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::PriceEdit *m_minPrice;
  AntiquaCRM::PriceEdit *m_normalPrice;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit BooksTabConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKS_CONFIGWIDGET_H
