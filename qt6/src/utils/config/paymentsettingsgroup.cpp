// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsettingsgroup.h"

#include <QLabel>
#include <QLayout>

PaymentSettingsGroup::PaymentSettingsGroup(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  // Tax and Currency Group
  QGroupBox *m_taxGroup = new QGroupBox(this);
  m_taxGroup->setTitle(tr("Payment settings"));

  QGridLayout *grLayout1 = new QGridLayout(m_taxGroup);
  grLayout1->setContentsMargins(5, 5, 5, 5);

  QLabel *m_info1 = new QLabel(this);
  m_info1->setIndent(2);
  m_info1->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_info1->setWordWrap(true);
  m_info1->setText(tr(
      "AntiquaCRM requires a valid currency specification for the calculation "
      "and printing of the invoices. By default, it recognizes the currency "
      "defined in the system. However, you can change your selling currency "
      "here and you can also set the TAX values in this Section."));
  grLayout1->addWidget(m_info1, 0, 2, 4, 1);
  grLayout1->setColumnStretch(2, 1);

  m_currency = new AntiquaCRM::SelectCurrency(m_taxGroup);
  m_currency->setObjectName("payment/currency");
  m_currency->setBuddyLabel(tr("Currency"));
  m_currency->appendStretch(1);
  grLayout1->addWidget(m_currency, 0, 0, 1, 2);

  m_vatNormal = new AntiquaCRM::NumEdit(m_taxGroup);
  m_vatNormal->setObjectName("payment/vat_normal");
  m_vatNormal->setBuddyLabel(tr("VAT (normal)"));
  m_vatNormal->setSuffix("%");
  m_vatNormal->appendStretch(1);
  grLayout1->addWidget(m_vatNormal, 1, 0, 1, 1);

  m_vatReduced = new AntiquaCRM::NumEdit(m_taxGroup);
  m_vatReduced->setObjectName("payment/vat_reduced");
  m_vatReduced->setBuddyLabel(tr("VAT (reduced)"));
  m_vatReduced->setSuffix("%");
  m_vatReduced->appendStretch(1);
  grLayout1->addWidget(m_vatReduced, 2, 0, 1, 1);

  grLayout1->setRowStretch(3, 1);
  m_taxGroup->setLayout(grLayout1);
  layout->addWidget(m_taxGroup);

  // Price Limits Group
  m_limitsGroup = new QGroupBox(this);
  m_limitsGroup->setTitle(tr("Price restrictions"));

  QGridLayout *grLayout2 = new QGridLayout(m_limitsGroup);
  grLayout2->setContentsMargins(5, 5, 5, 5);

  QLabel *m_info2 = new QLabel(this);
  m_info2->setIndent(2);
  m_info2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_info2->setWordWrap(true);
  m_info2->setText(
      tr("In this area you can set the price restrictions, which are set by "
         "default when creating a new article."));
  grLayout2->addWidget(m_info2, 0, 0, 1, 2);

  m_minPrice = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_minPrice->setObjectName("payment/price_limit_lowest");
  m_minPrice->setToolTip(tr("Price limit"));
  m_minPrice->setBuddyLabel(tr("Price minimal"));
  grLayout2->addWidget(m_minPrice, 1, 0, 1, 1);
  m_normalPrice = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_normalPrice->setObjectName("payment/price_limit_normal");
  m_normalPrice->setBuddyLabel(tr("Default"));
  m_normalPrice->setToolTip(m_normalPrice->toolTip());
  m_normalPrice->appendStretch(1);
  grLayout2->addWidget(m_normalPrice, 1, 1, 1, 1);

  m_minPrice->setValue(2);
  m_normalPrice->setValue(12);

  m_limitsGroup->setLayout(grLayout2);
  layout->addWidget(m_limitsGroup);

  // final
  setLayout(layout);

  // signals
  connect(m_currency, SIGNAL(sendInputChanged()), SLOT(updateCurrency()));
}

void PaymentSettingsGroup::updateCurrency() {
  QString _cyrrency = m_currency->getValue().toString();
  QList<AntiquaCRM::PriceEdit *> _l =
      findChildren<AntiquaCRM::PriceEdit *>(QString());
  for (int i = 0; i < _l.size(); i++) {
    _l.at(i)->updateCurrencySuffix(_cyrrency);
  }
}
