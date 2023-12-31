// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSSEARCHBAR_H
#define ANTIQUACRM_PLUGIN_BOOKSSEARCHBAR_H

#include <AntiquaInput>
#include <AntiquaTabs>
#include <QObject>
#include <QCheckBox>
#include <QPushButton>
#include <QWidget>

class BooksSelectFilter;

class ANTIQUACRM_LIBRARY BooksSearchBar final
    : public AntiquaCRM::TabsSearchBar {
  Q_OBJECT

private:
  BooksSelectFilter *m_selectFilter;
  AntiquaCRM::ALineEdit *m_searchInput;
  AntiquaCRM::ALineEdit *m_customSearch;
  QCheckBox *m_signed;
  QPushButton *m_searchBtn;

  const QString getTitleSearch(const QStringList &fields);
  bool lineInputsEnabled();
  void setCustomSearch(const QString &info = QString());

private Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit BooksSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  bool requiredLengthExists();
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKSSEARCHBAR_H
