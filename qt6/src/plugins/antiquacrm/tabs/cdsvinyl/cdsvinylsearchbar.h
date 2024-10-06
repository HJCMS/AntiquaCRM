// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLSEARCHBAR_H
#define ANTIQUACRM_PLUGIN_CDSVINYLSEARCHBAR_H

#include "cdsvinylconfig.h"
#include <AntiquaInput>
#include <AntiquaTabs>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class CDsVinylSelectFilter;

class ANTIQUACRM_CDSVINYL_PLUGIN CDsVinylSearchBar final : public AntiquaCRM::TabsSearchBar {
  Q_OBJECT

private:
  CDsVinylSelectFilter* m_selectFilter;
  AntiquaCRM::ALineEdit* m_searchInput;
  QPushButton* m_searchBtn;

  /**
   * @brief Prepare SQL-Titlesearch statement
   * @param fields - append Fields to WHERE clause
   */
  const QString getSearchString(const QStringList& fields);

  /**
   * @brief Check if one LineInput isEnabled
   */
  bool lineInputsEnabled();

private Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit CDsVinylSearchBar(QWidget* parent = nullptr);

  int searchLength() override;

  bool requiredLengthExists();

  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLSEARCHBAR_H
