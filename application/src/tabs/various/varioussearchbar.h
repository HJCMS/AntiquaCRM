// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSSEARCHBAR_H
#define ANTIQUACRM_VARIOUSSEARCHBAR_H

#include <AntiquaWidgets>
#include <AntiquaInputEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class VariousSearchEdit;

/**
 * @brief Inventory Various Search bar
 * @ingroup _various
 */
class VariousSearchBar final : public TabSearchBar {
  Q_OBJECT

private:
  QString p_search;
  VariousSearchEdit *m_searchEdit;
  QPushButton *m_searchBtn;

protected Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int index) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit VariousSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_VARIOUSSEARCHBAR_H
