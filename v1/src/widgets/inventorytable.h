// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#define ANTIQUACRM_INVENTORYTABLE_H

#include <QHeaderView>
#include <QMetaObject>
#include <QObject>
#include <QTableView>
#include <QWidget>

class InventoryTable : public QTableView {
  Q_OBJECT

private:
  int QueryLimit = 2500;

protected:
  QHeaderView *m_header;
  QString QueryHistory = QString();

  void setEnableTableViewSorting(bool);

  virtual bool sqlQueryTable(const QString &query) = 0;

protected Q_SLOTS:
  virtual void setSortByColumn(int column, Qt::SortOrder order) = 0;

public Q_SLOTS:
  void makeHistoryQuery();
  void setQueryLimit(int);

public:
  explicit InventoryTable(QWidget *parent = nullptr);
  int getQueryLimit();
  virtual bool initTable() = 0;
};

#endif // ANTIQUACRM_INVENTORYTABLE_H
