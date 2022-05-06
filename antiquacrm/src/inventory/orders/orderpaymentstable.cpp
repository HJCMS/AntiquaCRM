// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderpaymentstable.h"
#include "myicontheme.h"

#include <QAction>
#include <QDebug>
#include <QHash>
#include <QHeaderView>
#include <QMenu>
#include <QString>
#include <QTableWidgetItem>

OrderPaymentsTable::OrderPaymentsTable(QWidget *parent) : QTableWidget{parent} {
  setObjectName("order_payments_table");
  setColumnCount(6);
  addHeaderItem(0, "ID", tr("payment id"));
  addHeaderItem(1, tr("Article"), tr("article id"));
  addHeaderItem(2, tr("Price"), tr("selling price"));
  addHeaderItem(3, tr("Retail Price"), tr("Retail Price"));
  addHeaderItem(4, tr("Count"), tr("Article count"));
  addHeaderItem(5, tr("Summary"), tr("Article Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionHidden(0, true);
}

void OrderPaymentsTable::addHeaderItem(int i, const QString &name,
                                       const QString &tip) {
  QTableWidgetItem *item = new QTableWidgetItem(myIcon("autostart"), name,
                                                QTableWidgetItem::UserType);
  item->setToolTip(tip);
  setHorizontalHeaderItem(i, item);
}

void OrderPaymentsTable::removeTableItem() {
  QTableWidgetItem *item = currentItem();
  if (item != nullptr) {
    emit s_removeTableRow(item->row());
  }
}

void OrderPaymentsTable::contextMenuEvent(QContextMenuEvent *ev) {
  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_remove =
      m->addAction(myIcon("delete_table_row"), tr("delete selected article"));
  ac_remove->setObjectName("ac_context_remove_row");
  // ac_open->setEnabled(b);
  connect(ac_remove, SIGNAL(triggered()), this, SLOT(removeTableItem()));
  qDebug() << Q_FUNC_INFO;
  m->exec(ev->globalPos());
  delete m;
}

int OrderPaymentsTable::getArticleId(int row) const {
  QTableWidgetItem *item = QTableWidget::item(row, 0);
  if (item != nullptr)
    return item->text().toInt();

  return -1;
}
