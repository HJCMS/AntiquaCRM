// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSPAYMENTTABLE_H
#define ANTIQUACRM_ORDERSPAYMENTTABLE_H

#include <QContextMenuEvent>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVariant>
#include <QWidget>

class OrdersPaymentTable : public QTableWidget {
  Q_OBJECT

private:
  int articleTableCell = 1;
  void addHeaderItem(int, const QString &name, const QString &tip);

private Q_SLOTS:
  void removeTableItem();
  void checkChanged(QTableWidgetItem *, QTableWidgetItem *);

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void sendModified(bool);
  void sendRemoveTableRow(int row);

public:
  OrdersPaymentTable(QWidget *parent = nullptr);
  int getArticleId(int row) const;
};

#endif // ANTIQUACRM_ORDERSPAYMENTTABLE_H
