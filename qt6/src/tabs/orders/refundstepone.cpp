// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundstepone.h"
#include "orderstableview.h"

#include <QGroupBox>
#include <QLayout>

RefundStepOne::RefundStepOne(QWidget *parent) : RefundingPage{parent} {
  setObjectName("refund_stepone");
  QGridLayout *main_layout = new QGridLayout(this);
  // BEGIN:Row0
  QGroupBox *m_ids_group = new QGroupBox(this);
  m_ids_group->setTitle(tr("Order data"));
  QVBoxLayout *g_layout0 = new QVBoxLayout(m_ids_group);
  // Order Id
  m_oid = new AntiquaCRM::SerialId(m_ids_group);
  m_oid->setObjectName("o_id");
  m_oid->setBuddyLabel(tr("Order number"));
  m_oid->setRequired(true);
  g_layout0->addWidget(m_oid);
  // Invoice Id
  m_iid = new AntiquaCRM::SerialId(m_ids_group);
  m_iid->setObjectName("o_invoice_id");
  m_iid->setBuddyLabel(tr("Invoice number"));
  m_iid->setRequired(true);
  g_layout0->addWidget(m_iid);
  // Customer Id
  m_cid = new AntiquaCRM::SerialId(m_ids_group);
  m_cid->setObjectName("o_customer_id");
  m_cid->setBuddyLabel(tr("Customer number"));
  m_cid->setRequired(true);
  g_layout0->addWidget(m_cid);
  // Delivery Id
  m_did = new AntiquaCRM::TextLine(m_ids_group);
  m_did->setObjectName("o_delivery");
  m_did->setBuddyLabel(tr("Delivery number"));
  m_did->setRequired(true);
  m_did->setReadOnly(true);
  g_layout0->addWidget(m_did);
  g_layout0->addStretch(0);
  m_ids_group->setLayout(g_layout0);
  main_layout->addWidget(m_ids_group, 0, 0, 1, 1);
  // Invoice address group
  QGroupBox *m_addr_group = new QGroupBox(this);
  m_addr_group->setTitle(tr("Invoice address"));
  QVBoxLayout *g_layout1 = new QVBoxLayout(m_addr_group);
  QString css("QTextEdit {background: transparent;");
  css.append("border:none; border-top:1px solid palette(text);}");
  m_address = new AntiquaCRM::TextField(this);
  m_address->setObjectName("c_postal_address");
  m_address->setStyleSheet(css);
  g_layout1->addWidget(m_address);
  m_addr_group->setLayout(g_layout1);
  main_layout->addWidget(m_addr_group, 0, 1, 1, 1);
  // END:Row0

  // BEGIN:Row1
  m_table = new OrdersTableView(this, true);
  QStringList _hide_columns("a_payment_id");
  _hide_columns << "a_order_id";
  _hide_columns << "a_customer_id";
  _hide_columns << "a_modified";
  _hide_columns << "a_provider_id";
  m_table->hideColumns(_hide_columns);
  main_layout->addWidget(m_table, 1, 0, 1, 2);
  main_layout->setRowStretch(1, 1);
  // END:Row1

  setLayout(main_layout);
}

bool RefundStepOne::addDataRecord(qint64 id) {
  AntiquaCRM::ASqlFiles _tpl("query_order_by_oid");
  if (!_tpl.openTemplate() || id < 1)
    return false;

  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  _tpl.setWhereClause("o_id=" + QString::number(id));

  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() < 1)
    return false;

  const QSqlRecord _r = _q.record();
  while (_q.next()) {
    for (int i = 0; i < _r.count(); i++) {
      const QSqlField _f = _r.field(i);
      AntiquaCRM::AInputWidget *e =
          findChild<AntiquaCRM::AInputWidget *>(_f.name());
      if (e != nullptr)
        e->setValue(_q.value(_f.name()));
    }
  }
  _q.clear();

  QList<AntiquaCRM::OrderArticleItems> _list;
  QString sql("SELECT * FROM article_orders WHERE a_order_id=");
  sql.append(QString::number(id) + ";");
  QSqlQuery _query = m_sql->query(sql);
  if (_query.size() > 0) {
    const QSqlRecord _record = _query.record();
    while (_query.next()) {
      AntiquaCRM::OrderArticleItems _items;
      for (int i = 0; i < _record.count(); i++) {
        const QString _fn = _record.field(i).name();
        _items.append(
            AntiquaCRM::AProviderOrder::createItem(_fn, _query.value(_fn)));
      }
      _list.append(_items);
    }
  }
  if (_list.size() > 0)
    m_table->addArticles(_list);

  return true;
}
