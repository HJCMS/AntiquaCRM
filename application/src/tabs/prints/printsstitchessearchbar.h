// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRINTSSTITCHES_SEARCHBAR_H
#define ANTIQUACRM_PRINTSSTITCHES_SEARCHBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class PrintsStitchesFilterSelect;
class PrintsStitchesSearchLine;

class PrintsStitchesSearchBar final : public TabSearchBar {
  Q_OBJECT

private:
  int minLength = 2;
  QString p_search;
  PrintsStitchesFilterSelect *m_selectFilter;
  PrintsStitchesSearchLine *m_searchEdit;
  QPushButton *m_searchBtn;

protected Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int index) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit PrintsStitchesSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_PRINTSSTITCHES_SEARCHBAR_H