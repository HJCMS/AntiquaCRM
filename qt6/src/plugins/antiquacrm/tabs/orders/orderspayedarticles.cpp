// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderspayedarticles.h"

OrdersPayedArticles::OrdersPayedArticles(QWidget *parent) : QWidget{parent} {
  setObjectName("orders_item_list");
  setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QString tbInfo = tr("Current Article orders");
  layout->addWidget(new QLabel(tbInfo + ":", this));

  m_table = new AntiquaCRM::OrdersTableView(this, false);
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

void OrdersPayedArticles::clearTable() { m_table->clearContents(); }

void OrdersPayedArticles::insertArticle(
    const AntiquaCRM::OrderArticleItems &item) {
  m_table->addArticle(item);
}

void OrdersPayedArticles::setArticleChanged(bool b) {
  setWindowModified(b);
  if (b == ArticleChanged)
    return;

  ArticleChanged = b;
  emit articleChanged(ArticleChanged);
}

bool OrdersPayedArticles::isEmpty() { return (m_table->rowCount() < 1); }

bool OrdersPayedArticles::getArticleChanged() { return ArticleChanged; }

bool OrdersPayedArticles::setArticleOrderId(qint64 oid) {
  if (oid < 1)
    return false;

  return m_table->setArticleOrderId(oid);
}

bool OrdersPayedArticles::importArticles(
    const QList<AntiquaCRM::OrderArticleItems> &list) {
  return m_table->setOrderArticles(list);
}

const QStringList OrdersPayedArticles::getQueryData() {
  return m_table->getSqlQuery();
}
