// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESSEARCHBAR_H
#define ANTIQUACRM_PLUGIN_STITCHESSEARCHBAR_H

#include <AntiquaInput>
#include <AntiquaTabs>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class StitchesSelectFilter;
class StitchesSearchLine;

class ANTIQUACRM_LIBRARY StitchesSearchBar final
    : public AntiquaCRM::TabsSearchBar {
  Q_OBJECT

private:
  StitchesSelectFilter *m_selectFilter;
  AntiquaCRM::ALineEdit *m_searchInput;
  AntiquaCRM::ALineEdit *m_customSearch;
  QPushButton *m_searchBtn;

  /**
   * @brief Prepare SQL-Titlesearch statement
   * @param fields - append Fields to WHERE clause
   */
  const QString getTitleSearch(const QStringList &fields);

  /**
   * @brief Check if one LineInput isEnabled
   */
  bool lineInputsEnabled();

  /**
   * @brief Custom Search Input
   * @param info - QLineEdit::PlaceholderText
   */
  void setCustomSearch(const QString &info = QString());

private Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit StitchesSearchBar(QWidget *parent = nullptr);

  int searchLength() override;

  bool requiredLengthExists();

  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PLUGIN_STITCHESSEARCHBAR_H
