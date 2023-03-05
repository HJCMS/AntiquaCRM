// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVSEARCHBAR_H
#define ANTIQUACRM_CDVSEARCHBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class CDVFilterSelect;
class CDVSearchLine;

class CDVSearchBar final : public TabSearchBar {
  Q_OBJECT

private:
  QString p_search;
  CDVFilterSelect *m_selectFilter;
  CDVSearchLine *m_searchEdit;
  QPushButton *m_searchBtn;

protected Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int index) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit CDVSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_CDVSEARCHBAR_H
