// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundfinalize.h"
#include "orderstableview.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QLayout>

RefundFinalize::RefundFinalize(QWidget *parent) : RefundingPage{parent} {
  setObjectName("refund_steptwo");

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_refundings = new OrdersTableView(this, true);
  QStringList _hide_columns("a_payment_id");
  _hide_columns << "a_order_id";
  _hide_columns << "a_customer_id";
  _hide_columns << "a_price";
  _hide_columns << "a_modified";
  _hide_columns << "a_provider_id";
  m_refundings->hideColumns(_hide_columns);
  layout->addWidget(m_refundings);

  m_refundCost = new AntiquaCRM::PriceEdit(this);
  m_refundCost->setObjectName("a_refunds_cost");
  m_refundCost->setMinimum(0.00);
  m_refundCost->setSingleStep(0.10);
  m_refundCost->setBuddyLabel(tr("Setting refundcost"));
  m_refundCost->appendStretch();
  layout->addWidget(m_refundCost);
  layout->addStretch(1);

  setLayout(layout);

  connect(m_refundCost, SIGNAL(sendInputChanged()), SLOT(refundsCostChanged()));
}

void RefundFinalize::refundsCostChanged() {
  double _price = m_refundCost->getValue().toDouble();
  m_refundings->updateRefundCoast(_price);
}

void RefundFinalize::addArticles(const QList<qint64> &list) {
  if (list.size() < 1)
    return;

  QStringList _ids;
  QListIterator<qint64> _it(list);
  while (_it.hasNext()) {
    qint64 _i = _it.next();
    if (_i < 1)
      continue;

    _ids << QString::number(_i);
  }

  // Begin:QueryRefundArticles
  QString _sql("SELECT * FROM article_orders WHERE a_payment_id IN (");
  _sql.append(_ids.join(",") + ") AND ");
  _sql.append("a_order_id=" + QString::number(p_order_id) + ";");

  QSqlQuery _query1 = m_sql->query(_sql);
  if (_query1.size() < 1)
    return;

  QList<AntiquaCRM::OrderArticleItems> _articles;
  const QSqlRecord _rec1 = _query1.record();
  while (_query1.next()) {
    AntiquaCRM::OrderArticleItems _l;
    for (int i = 0; i < _rec1.count(); i++) {
      const QString _f = _rec1.field(i).name();
      QVariant _v = _query1.value(_f);
      if (_f == "a_refunds_cost") {
        m_refundCost->setRestrictions(_rec1.field(i));
        _v = m_refundCost->getValue().toDouble();
      }
      _l.append(AntiquaCRM::AProviderOrder::createItem(_f, _v));
    }
    _articles.append(_l);
  }
  _query1.clear();
  // End:QueryRefundArticles

  if (_articles.size() < 1)
    return;

  m_refundings->addArticles(_articles);
  m_refundCost->setValue(refundsCost());
}

bool RefundFinalize::initPageData(AntiquaCRM::ASqlCore *con, qint64 id) {
  if (m_sql == nullptr)
    m_sql = con;

  p_order_id = id;
  return true;
}
