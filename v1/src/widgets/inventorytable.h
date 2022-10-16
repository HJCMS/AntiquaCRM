// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#define ANTIQUACRM_INVENTORYTABLE_H

#include <AntiquaCRM>
#include <QHeaderView>
#include <QMetaObject>
#include <QModelIndex>
#include <QObject>
#include <QTableView>
#include <QWidget>

class InventoryTable : public QTableView {
  Q_OBJECT

private:
  int QueryLimit;

protected:
  AntiquaCRM::ASettings *m_cfg;
  QHeaderView *m_header;
  QString QueryHistory = QString();

  void setEnableTableViewSorting(bool);

  virtual qint64 getArticleID(const QModelIndex &index) = 0;

  virtual bool sqlQueryTable(const QString &query) = 0;

protected Q_SLOTS:
  virtual void setSortByColumn(int column, Qt::SortOrder order) = 0;
  virtual void getSqlModelError(const QString &table,
                                const QString &message) = 0;
  virtual void getSelectedItem(const QModelIndex &index) = 0;

public Q_SLOTS:
  virtual void setReloadView() = 0;
  void setQueryLimit(int);

Q_SIGNALS:
  void sendQueryReport(const QString &report);
  void sendCopyToClibboard(const QString &str);
  void sendOpenEntry(qint64 articleId);
  void sendArticleId(qint64 articleId);
  void sendCreateNewEntry();

public:
  explicit InventoryTable(QWidget *parent = nullptr);
  int getQueryLimit();
  virtual bool setQuery(const QString &clause = QString()) = 0;
};

#endif // ANTIQUACRM_INVENTORYTABLE_H
