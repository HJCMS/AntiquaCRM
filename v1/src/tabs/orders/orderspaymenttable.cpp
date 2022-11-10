// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderspaymenttable.h"

#include <QAction>
#include <QDebug>
#include <QHash>
#include <QHeaderView>
#include <QIcon>
#include <QMenu>
#include <QString>
#include <QTableWidgetItem>

OrdersPaymentTable::OrdersPaymentTable(QWidget *parent) : QTableWidget{parent} {
  setObjectName("order_payments_table");
  setColumnCount(7);
  addHeaderItem(0, "ID", tr("payment id"));
  articleTableCell = 1;
  addHeaderItem(1, tr("Article"), tr("article id"));
  addHeaderItem(2, tr("Price"), tr("selling price"));
  addHeaderItem(3, tr("Retail Price"), tr("Retail Price"));
  addHeaderItem(4, tr("Count"), tr("Article count"));
  addHeaderItem(5, tr("Type"), tr("Article Type"));
  addHeaderItem(6, tr("Summary"), tr("Article Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionHidden(0, true);

  connect(this,
          SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)),
          this, SLOT(checkChanged(QTableWidgetItem *, QTableWidgetItem *)));
}

void OrdersPaymentTable::addHeaderItem(int i, const QString &name,
                                       const QString &tip) {
  QTableWidgetItem *item = new QTableWidgetItem(
      QIcon("://icons/edit.png"), name, QTableWidgetItem::UserType);
  item->setToolTip(tip);
  setHorizontalHeaderItem(i, item);
}

void OrdersPaymentTable::removeTableItem() {
  QTableWidgetItem *item = currentItem();
  if (item != nullptr) {
    emit s_removeTableRow(item->row());
  }
}

void OrdersPaymentTable::checkChanged(QTableWidgetItem *, QTableWidgetItem *) {
  emit hasModified(true);
}

void OrdersPaymentTable::contextMenuEvent(QContextMenuEvent *ev) {
  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_remove = m->addAction(QIcon("://icons/db_remove.png"),
                                    tr("delete selected article"));
  ac_remove->setObjectName("ac_context_remove_row");
  connect(ac_remove, SIGNAL(triggered()), this, SLOT(removeTableItem()));
  m->exec(ev->globalPos());
  delete m;
}

int OrdersPaymentTable::getArticleId(int row) const {
  QTableWidgetItem *item = QTableWidget::item(row, articleTableCell);
  if (item != nullptr)
    return item->text().toInt();

  return -1;
}
