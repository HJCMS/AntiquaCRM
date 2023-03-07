// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSSEARCHBAR_H
#define ANTIQUACRM_CUSTOMERSSEARCHBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class CustomersSelectFilter;
class CustomersSearchLineEdit;

/**
 * @brief Customers Search bar
 * @ingroup AntiquaCustomers
 */
class CustomersSearchBar final : public TabSearchBar {
  Q_OBJECT

private:
  CustomersSelectFilter *m_selectFilter;
  CustomersSearchLineEdit *m_searchEdit;
  QPushButton *m_searchBtn;

protected Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int index) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit CustomersSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_CUSTOMERSSEARCHBAR_H
