// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLE_H
#define ANTIQUACRM_PURCHASETABLE_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QTableView>
#include <QContextMenuEvent>

class PurchaseTableModel;
class PurchaseTableDelegate;

class PurchaseTable final : public QTableView {
  Q_OBJECT

private:
  const bool readOnlyMode;
  AntiquaCRM::ASqlCore *m_sql;
  PurchaseTableModel *m_model;
  PurchaseTableDelegate *m_delegate;
  QHeaderView *m_header;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void removeCurrentRow();

Q_SIGNALS:
  void sendEntriesValid(bool);

public Q_SLOTS:
  void setQueryId(qint64 id, const QString &field = QString("a_order_id"));

public:
  explicit PurchaseTable(QWidget *parent = nullptr, bool readOnly = false);
  bool save();
  int rowCount() const;
  bool updateRows(qint64 oId, qint64 cId);
  void hideColumns(const QList<int> columns);
  bool addRow(const AntiquaCRM::OrderArticleItems &items);
  const AntiquaCRM::OrderArticleItems getRow(int row);
};

#endif // ANTIQUACRM_PURCHASETABLE_H
