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

class BooksTabConfigWidget final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit BooksTabConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;
  const QIcon getIcon() const override;
  const QString getTitle() const override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKS_CONFIGWIDGET_H
