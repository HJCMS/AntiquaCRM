// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "returnedit.h"
#include "buttonnext.h"
#include "purchasetable.h"

#include <AntiquaCRM>
#include <QLayout>
#include <QRegExpValidator>
#include <QSqlField>
#include <QSqlRecord>

ReturnEdit::ReturnEdit(QWidget *parent) : QWidget{parent} {
  int row = 0;

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);

  o_id = new SerialID(this);
  o_id->setObjectName("o_id");
  o_id->setInfo(tr("Order Id"));
  layout->addWidget(o_id, row++, 0, 1, 1);

  o_invoice_id = new SerialID(this);
  o_invoice_id->setObjectName("o_invoice_id");
  o_invoice_id->setInfo(tr("Invoice Id"));
  layout->addWidget(o_invoice_id, row++, 0, 1, 1);

  o_customer_id = new SerialID(this);
  o_customer_id->setObjectName("o_customer_id");
  o_customer_id->setInfo(tr("Customer Id"));
  layout->addWidget(o_customer_id, row++, 0, 1, 1);

  o_delivery = new DeliveryId(this);
  o_delivery->setObjectName("o_delivery");
  o_delivery->setInfo(tr("Delivery note"));
  layout->addWidget(o_delivery, row++, 0, 1, 1);

  c_postal_address = new AddressInfo(this);
  c_postal_address->setObjectName("c_postal_address");
  layout->addWidget(c_postal_address, 0, 1, row++, 1);

  QStringList hideColumns;
  hideColumns << "a_order_id";
  hideColumns << "a_customer_id";
  hideColumns << "a_modified";
  hideColumns << "a_provider_id";

  m_table = new PurchaseTable(this, false);
  m_table->hideColumns(hideColumns);
  layout->addWidget(m_table, row++, 0, 1, 2);

  QLabel *lbinfo = new QLabel(
      tr("Double click to Article insert payment IDs that were returned."),
      this);
  layout->addWidget(lbinfo, row++, 0, 1, 2);

  m_payment_ids = new LineEdit(this);
  m_payment_ids->setObjectName("returned_payments");
  m_payment_ids->setInfo(tr("Payment IDs"));
  m_payment_ids->setReadOnly(true);
  QSqlField f("returned_payments", QVariant::String);
  f.setRequired(true); // hide clear button
  m_payment_ids->setProperties(f);
  layout->addWidget(m_payment_ids, row++, 0, 1, 2, Qt::AlignLeft);

  btn_next = new ButtonNext(this);
  layout->addWidget(btn_next, row++, 0, 1, 2);

  layout->setRowStretch(row++, 1);
  setLayout(layout);

  connect(btn_next, SIGNAL(clicked()), SIGNAL(sendReady()));
  connect(m_table, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(addPaymentId(const QModelIndex &)));
}

void ReturnEdit::addPaymentId(const QModelIndex &index) {
  if (!index.isValid())
    return;

  int id = m_table->getArticlePaymentId(index.row());
  QString str_id = QString::number(id);
  QStringList ids;
  QStringList list = m_payment_ids->value().toString().split(",");
  foreach (QString id, list) {
    if (id.isEmpty())
      continue;

    ids << id;
  }
  if (!ids.contains(str_id))
    ids << str_id;

  m_payment_ids->setValue(ids.join(","));
}

void ReturnEdit::loadArticleData(const QSqlQuery &query) {
  QSqlQuery q(query);
  if (q.size() > 0) {
    QList<AntiquaCRM::OrderArticleItems> articles;
    QSqlRecord r = query.record();
    while (q.next()) {
      AntiquaCRM::OrderArticleItems article;
      for (int c = 0; c < r.count(); c++) {
        AntiquaCRM::ArticleOrderItem item;
        item.key = r.field(c).name();
        item.value = q.value(item.key);
        article.append(item);
      }
      articles.append(article);
    }
    if (articles.size() > 0)
      m_table->setOrderArticles(articles);
  }
}

const QStringList ReturnEdit::getRefundIds() {
  QString str_ids = m_payment_ids->value().toString();
  if (str_ids.isEmpty()) {
    emit sendMessage(tr("No Payment Id has been add!"));
    return QStringList();
  }
  return str_ids.split(",");
}
