// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundstepone.h"

#include <QGroupBox>
#include <QLayout>

RefundStepOne::RefundStepOne(QWidget *parent) : RefundingPage{parent} {
  setObjectName("refund_stepone");
  QGridLayout *main_layout = new QGridLayout(this);
  // Begin:Group
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
  // End:Group

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

  // Todo Table
  main_layout->setRowStretch(1, 1);

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

  return true;
}
