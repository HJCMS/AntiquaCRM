// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersbillings.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

CustomersBillings::CustomersBillings(QWidget *parent) : QWidget{parent} {
  setObjectName("CustomerBillingInfo");
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("billing_info_layout");

  QGridLayout *row1 = new QGridLayout();
  row1->setObjectName("billing_info_row1");

  c_locked = new BoolBox(this);
  c_locked->setObjectName("c_locked");
  c_locked->setInfo(tr("Locked"));
  row1->addWidget(c_locked, 0, 0, 1, 1);

  c_trusted = new QComboBox(this);
  c_trusted->setObjectName("c_trusted");
  // c_trusted->setInfo("Trusted");
  row1->addWidget(c_trusted, 0, 1, 1, 1);

  c_tax_id = new LineEdit(this);
  c_tax_id->setObjectName("c_tax_id");
  c_tax_id->setInfo(tr("Tax Id."));
  row1->addWidget(c_tax_id, 0, 2, 1, 1);

  c_iban = new LineEdit(this);
  c_iban->setObjectName("c_iban");
  c_iban->setInfo(tr("IBAN Ident"));
  row1->addWidget(c_iban, 1, 0, 1, 2);

  c_swift_bic = new LineEdit(this);
  c_swift_bic->setObjectName("c_swift_bic");
  c_swift_bic->setInfo(tr("BIC Swift Code"));
  row1->addWidget(c_swift_bic, 1, 2, 1, 1);

  QLabel *infoComments = new QLabel(this);
  infoComments->setText(tr("Comments"));
  row1->addWidget(infoComments, 2, 0, 1, 1);

  c_shipping_email = new EMailEdit(this);
  c_shipping_email->setObjectName("c_shipping_email");
  c_shipping_email->setInfo(tr("Billing E-Mail"));
  row1->addWidget(c_shipping_email, 2, 1, 1, 2);

  c_comments = new TextField(this);
  c_comments->setObjectName("c_comments");
  row1->addWidget(c_comments, 3, 0, 1, 3);

  layout->addLayout(row1);

  setLayout(layout);
}
