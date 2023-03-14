// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersitemlist.h"
#include "purchasetable.h"

#include <AGlobal>
#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QListWidgetItem>
#include <QPushButton>

OrdersItemList::OrdersItemList(QWidget *parent) : QWidget{parent} {
  setObjectName("orders_item_list");
  setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QString tbInfo = tr("Current Article orders");
  layout->addWidget(new QLabel(tbInfo + ":", this));

  m_table = new PurchaseTable(this, false);
  layout->addWidget(m_table);
  layout->addStretch(1);
  setLayout(layout);

/**
 * Für den Kunden ausblenden. Werden hier nicht benötigt!
 * @warning Die Spaltenzahl ist zu diesem Zeitpunkt noch nicht bekannt!
 *          Deshalb wird die Konstante „table_columns“ heran gezogen.
 */
#ifndef ANTIQUA_DEVELOPEMENT
  QStringList hideColumn("a_payment_id");
  hideColumn << "a_order_id";
  hideColumn << "a_customer_id";
  hideColumn << "a_modified";
  hideColumn << "a_provider_id";
  hideColumn << "a_refunds_cost";
  m_table->hideColumns(hideColumn);
#endif

  connect(m_table, SIGNAL(sendTableModified(bool)),
          SLOT(setArticleChanged(bool)));
}

void OrdersItemList::clearTable() { m_table->clearTable(); }

void OrdersItemList::insertArticle(const AntiquaCRM::OrderArticleItems &item) {
  m_table->addOrderArticle(item);
}

void OrdersItemList::setArticleChanged(bool b) {
  setWindowModified(b);
  if (b == ArticleChanged)
    return;

  ArticleChanged = b;
  emit articleChanged(ArticleChanged);
}

bool OrdersItemList::isEmpty() { return (m_table->rowCount() < 1); }

bool OrdersItemList::getArticleChanged() { return ArticleChanged; }

bool OrdersItemList::setArticleOrderId(qint64 oid) {
  if (oid < 1)
    return false;

  return m_table->setArticleOrderId(oid);
}

bool OrdersItemList::importArticles(
    const QList<AntiquaCRM::OrderArticleItems> &list) {
  bool status = m_table->setOrderArticles(list);
  return status;
}

const QStringList OrdersItemList::getQueryData() {
  return m_table->getSqlQuery();
}
