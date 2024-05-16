// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSSEARCHBAR_H
#define ANTIQUACRM_PLUGIN_ORDERSSEARCHBAR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>
#include <QPushButton>

class ANTIQUACRM_LIBRARY OrdersSearchBar final
    : public AntiquaCRM::TabsSearchBar {
  Q_OBJECT

private:
  int min_length = 1;
  const QDate cDate;
  mutable QString p_search;
  AntiquaCRM::AComboBox *m_filter;
  AntiquaCRM::ALineEdit *m_searchInput;
  AntiquaCRM::AComboBox *m_datePart;
  AntiquaCRM::NumEdit *m_year;
  QPushButton *m_searchBtn;

public Q_SLOTS:
  void setSearch() override;
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit OrdersSearchBar(QWidget *patent = nullptr);
  const QString past12Months() const;
  int getYear();
  const QString getDatePart();
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERSSEARCHBAR_H
