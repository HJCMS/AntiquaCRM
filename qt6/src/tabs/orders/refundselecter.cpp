// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundselecter.h"
#include "orderstableview.h"

#include <QGroupBox>
#include <QLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

RefundSelecter::RefundSelecter(QWidget *parent)
    : RefundingPage{parent}, p_id_pattern{"([0-9]+\\,?)+"} {
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
#ifndef ANTIQUA_DEVELOPEMENT
  m_table->hideColumns(_hide_columns);
#endif
  m_table->setMinimumHeight(100);
  main_layout->addWidget(m_table, 1, 0, 1, 2);
  main_layout->setRowStretch(1, 1);
  // END:Row1

  // BEGIN:Row2
  const QString _info =
      tr("Selecting article refunds from table with double click on it.");
  m_refundsIds = new QLineEdit(this);
  m_refundsIds->setObjectName("reunding_id_list");
  m_refundsIds->setPlaceholderText(_info);
  m_refundsIds->setToolTip(_info);
  m_refundsIds->setClearButtonEnabled(true);
  m_refundsIds->setValidator(
      new QRegularExpressionValidator(p_id_pattern, m_refundsIds));
  main_layout->addWidget(m_refundsIds, 2, 0, 1, 2);
  // END:Row2
  setLayout(main_layout);

  // Signals
  connect(m_table, SIGNAL(sendPaymentId(qint64)),
          SLOT(addPaymentRefund(qint64)));
  connect(m_refundsIds, SIGNAL(textChanged(const QString &)),
          SLOT(refundParser(const QString &)));
}

void RefundSelecter::refundParser(const QString &data) {
  bool _status = false;
  if (data.trimmed().length() > 0)
    _status = data.contains(p_id_pattern);

  emit sendStatus(_status);
}

void RefundSelecter::addPaymentRefund(qint64 id) {
  QString _number = QString::number(id);
  QString _current = m_refundsIds->text().trimmed();
  if (_current.contains(_number))
    return;

  if (_current.size() > 0) {
    _current.append("," + _number);
    m_refundsIds->setText(_current);
  } else {
    m_refundsIds->setText(_number);
  }
}

bool RefundSelecter::initPageData(AntiquaCRM::ASqlCore *con, qint64 id) {
  if (m_sql == nullptr)
    m_sql = con;

  // Begin:QueryOrderData
  AntiquaCRM::ASqlFiles _tpl("query_order_by_oid");
  if (!_tpl.openTemplate() || id < 1)
    return false;

  _tpl.setWhereClause("o_id=" + QString::number(id));
  QSqlQuery _query0 = m_sql->query(_tpl.getQueryContent());
  if (_query0.size() < 1)
    return false;

  const QSqlRecord _rec0 = _query0.record();
  while (_query0.next()) {
    for (int i = 0; i < _rec0.count(); i++) {
      const QSqlField _f = _rec0.field(i);
      AntiquaCRM::AInputWidget *e =
          findChild<AntiquaCRM::AInputWidget *>(_f.name());
      if (e != nullptr)
        e->setValue(_query0.value(_f.name()));
    }
  }
  _query0.clear();
  // End:QueryOrderData

  // Begin:QueryOrderArticles
  QString sql("SELECT * FROM article_orders WHERE a_order_id=");
  sql.append(QString::number(id) + ";");
  QSqlQuery _query1 = m_sql->query(sql);
  if (_query1.size() < 1)
    return false;

  QList<AntiquaCRM::OrderArticleItems> _articles;
  const QSqlRecord _rec1 = _query1.record();
  while (_query1.next()) {
    AntiquaCRM::OrderArticleItems _l;
    for (int i = 0; i < _rec1.count(); i++) {
      const QString _f = _rec1.field(i).name();
      _l.append(AntiquaCRM::AProviderOrder::createItem(_f, _query1.value(_f)));
    }
    _articles.append(_l);
  }
  _query1.clear();
  // End:QueryOrderArticles

  if (_articles.size() > 0) {
    m_table->addArticles(_articles);
    return true;
  }
  return false;
}

const QList<qint64> RefundSelecter::selectedRefunds() {
  QList<qint64> _l;
  foreach (QString _s, m_refundsIds->text().split(",")) {
    bool _b;
    qint64 _i = _s.toLongLong(&_b, 0);
    if (_b && _i > 0)
      _l.append(_i);
  }
  return _l;
}
