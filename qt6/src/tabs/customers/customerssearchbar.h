// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSSEARCHBAR_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSSEARCHBAR_H

#include <AntiquaInput>
#include <AntiquaTabs>
#include <QObject>
#include <QCheckBox>
#include <QPushButton>
#include <QWidget>

class CustomersSelectFilter;

class ANTIQUACRM_LIBRARY CustomersSearchBar final
    : public AntiquaCRM::TabsSearchBar {
  Q_OBJECT

private:
  CustomersSelectFilter *m_filter;
  AntiquaCRM::ALineEdit *m_input;
  QPushButton *m_searchBtn;

  const QString getSearchString(const QStringList &fields);

private Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit CustomersSearchBar(QWidget *parent = nullptr);

  int searchLength() override;

  bool requiredLengthExists();

  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSSEARCHBAR_H
