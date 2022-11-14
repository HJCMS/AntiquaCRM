// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetableheader.h"

#include <QIcon>
#include <QMap>

PurchaseTableHeader::PurchaseTableHeader(QWidget *parent)
    : QHeaderView{Qt::Horizontal, parent} {
  setSectionsMovable(false);
  setHighlightSections(true);
  setSectionResizeMode(QHeaderView::ResizeToContents);
  setStretchLastSection(true);
}

const QString PurchaseTableHeader::columnTitle(const QString &fieldName) {
  QMap<QString, QString> m;
  m.insert("a_payment_id", tr("Payment Id"));
  m.insert("a_order_id", tr("Order Id"));
  m.insert("a_article_id", tr("Article Id"));
  m.insert("a_customer_id", tr("Customer Id"));
  m.insert("a_count", tr("Count"));
  m.insert("a_title", tr("Title"));
  m.insert("a_price", tr("Price"));
  m.insert("a_sell_price", tr("Sell Price"));
  m.insert("a_modified", tr("Modified"));
  m.insert("a_provider_id", tr("Provider Id"));
  m.insert("a_type", tr("Type"));
  return m.value(fieldName);
}

const QList<int> PurchaseTableHeader::editableColumns() {
  QList<int> l;
  l.append(4);
  l.append(6);
  l.append(7);
  l.append(10);
  return l;
}
