// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#define ANTIQUACRM_INVENTORYTABLE_H

#include <AntiquaCRM>
#include <QHeaderView>
#include <QJsonObject>
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

  virtual qint64 getTableID(const QModelIndex &index) = 0;

  virtual bool sqlQueryTable(const QString &query) = 0;

protected Q_SLOTS:
  void getSqlModelError(const QString &table, const QString &message);

  virtual void setSortByColumn(int column, Qt::SortOrder order) = 0;
  virtual void getSelectedItem(const QModelIndex &index) = 0;
  virtual void createOpenEntry() = 0;
  virtual void createCopyClipboard() = 0;
  virtual void createOrderSignal() = 0;

public Q_SLOTS:
  virtual void setReloadView() = 0;
  void setQueryLimit(int);

Q_SIGNALS:
  void sendSocketOperation(const QJsonObject &);
  void sendResultExists(bool);
  void sendQueryReport(const QString &report);
  void sendCopyToClibboard(const QString &str);
  void sendOpenEntry(qint64 articleId);
  void sendCurrentId(qint64 sectionId);
  void sendCreateNewEntry();

public:
  explicit InventoryTable(QWidget *parent = nullptr);
  static const QIcon cellIcon(const QString &name);
  int getQueryLimit();
  virtual int rowCount() = 0;
  virtual bool setQuery(const QString &clause = QString()) = 0;
};

#endif // ANTIQUACRM_INVENTORYTABLE_H
