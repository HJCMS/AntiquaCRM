// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundfinalize.h"
#include "orderstableview.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QLayout>
#include <QList>

RefundFinalize::RefundFinalize(QWidget *parent) : RefundingPage{parent} {
  setObjectName("refund_steptwo");

  QGridLayout *layout = new QGridLayout(this);

  m_refundTable = new OrdersTableView(this, true);
  QStringList _hide_columns("a_payment_id");
  _hide_columns << "a_order_id";
  _hide_columns << "a_customer_id";
  _hide_columns << "a_price";
  _hide_columns << "a_modified";
  _hide_columns << "a_provider_id";
  m_refundTable->hideColumns(_hide_columns);
  layout->addWidget(m_refundTable, 0, 1, 1, 2);

  m_refundCost = new AntiquaCRM::PriceEdit(this);
  m_refundCost->setObjectName("a_refunds_cost");
  m_refundCost->setMinimum(0.00);
  m_refundCost->setSingleStep(0.10);
  m_refundCost->setBuddyLabel(tr("Setting refundcost"));
  m_refundCost->appendStretch();
  layout->addWidget(m_refundCost, 1, 1, 1, 1);

  m_divideCost = new QCheckBox(this);
  m_divideCost->setText(tr("Dividing refunds cost to article count."));
  m_divideCost->setChecked(true);
  layout->addWidget(m_divideCost, 1, 2, 1, 1);
  layout->setRowStretch(2, 1);

  setLayout(layout);

  connect(m_refundCost, SIGNAL(sendInputChanged()), SLOT(refundsCostChanged()));
  connect(m_divideCost, SIGNAL(released()), SLOT(refundsCostChanged()));
}

void RefundFinalize::refundsCostChanged() {
  double _price = m_refundCost->getValue().toDouble();
  if (m_divideCost->isChecked()) {
    _price = (_price / m_refundTable->rowCount());
  }
  m_refundTable->updateRefundCoast(_price);
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
  AntiquaCRM::ASqlFiles _tpl("query_build_refunding");
  if (!_tpl.openTemplate() || p_order_id < 1)
    return;

  _tpl.setWhereClause("a_payment_id IN (" + _ids.join(",") + ")");

  QSqlQuery _query1 = m_sql->query(_tpl.getQueryContent());
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

  if (_articles.size() < 1) {
    // disable apply button
    emit sendEnableButton(false);
    return;
  }

  m_refundTable->addArticles(_articles);
  // Setting defaults
  m_refundCost->setValue(refundingCost());
  // Enable apply button
  emit sendEnableButton(true);
}

bool RefundFinalize::initPageData(AntiquaCRM::ASqlCore *con, qint64 id) {
  if (m_sql == nullptr)
    m_sql = con;

  p_order_id = id;
  return true;
}

const QList<AntiquaCRM::OrderArticleItems> RefundFinalize::getFinalRefunding() {
  return m_refundTable->getRefundData();
}
