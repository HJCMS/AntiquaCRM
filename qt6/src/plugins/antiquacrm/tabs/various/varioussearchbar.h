// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUSSEARCHBAR_H
#define ANTIQUACRM_PLUGIN_VARIOUSSEARCHBAR_H

#include "variousconfig.h"
#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QPushButton>
#include <QWidget>

class ANTIQUACRM_VARIOUS_PLUGIN VariousSearchBar final : public AntiquaCRM::TabsSearchBar {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit* m_searchLine;
  QPushButton* m_searchButton;

private Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit VariousSearchBar(QWidget* parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PLUGIN_VARIOUSSEARCHBAR_H
