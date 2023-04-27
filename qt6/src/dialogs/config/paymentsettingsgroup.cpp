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
  // m_taxGroup->setStyleSheet("QFrame {border: 1px solid red;}");

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
  // m_limitsGroup->setStyleSheet("QFrame {border: 1px solid red;}");

  QGridLayout *grLayout2 = new QGridLayout(m_limitsGroup);
  grLayout2->setContentsMargins(5, 5, 5, 5);

  QLabel *m_info2 = new QLabel(this);
  m_info2->setIndent(2);
  m_info2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_info2->setWordWrap(true);
  m_info2->setText(
      tr("In this area you can set the price restrictions, which are set by "
         "default when creating a new article. There are 3 Categories: Books, "
         "Media and „Prints + Stitches or Engravings“. You can set a minimum "
         "and default value for each category."));
  grLayout2->addWidget(m_info2, 0, 2, 4, 1);
  grLayout2->setColumnStretch(2, 1);

  m_minBook = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_minBook->setObjectName("payment/limits_book_low");
  m_minBook->setToolTip(tr("Books and Magazines"));
  m_minBook->setBuddyLabel(tr("Books min."));
  grLayout2->addWidget(m_minBook, 0, 0, 1, 1);
  m_normalBook = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_normalBook->setObjectName("payment/limits_book_normal");
  m_normalBook->setBuddyLabel(tr("Default"));
  m_normalBook->setToolTip(m_minBook->toolTip());
  grLayout2->addWidget(m_normalBook, 0, 1, 1, 1);

  m_minMedia = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_minMedia->setObjectName("payment/limits_media_low");
  m_minMedia->setToolTip(tr("DVD/CDs and Vinyl"));
  m_minMedia->setBuddyLabel(tr("Media min."));
  grLayout2->addWidget(m_minMedia, 1, 0, 1, 1);
  m_normalMedia = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_normalMedia->setObjectName("payment/limits_media_normal");
  m_normalMedia->setBuddyLabel(tr("Default"));
  m_normalMedia->setToolTip(m_minMedia->toolTip());
  grLayout2->addWidget(m_normalMedia, 1, 1, 1, 1);

  m_minPrints = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_minPrints->setObjectName("payment/limits_prints_low");
  m_minPrints->setToolTip(tr("Prints and Stitches"));
  m_minPrints->setBuddyLabel(tr("Prints min."));
  grLayout2->addWidget(m_minPrints, 2, 0, 1, 1);
  m_normalPrints = new AntiquaCRM::PriceEdit(m_limitsGroup);
  m_normalPrints->setObjectName("payment/limits_prints_normal");
  m_normalPrints->setBuddyLabel(tr("Default"));
  m_normalPrints->setToolTip(m_minPrints->toolTip());
  grLayout2->addWidget(m_normalPrints, 2, 1, 1, 1);

  grLayout2->setRowStretch(3, 1);
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
    AntiquaCRM::PriceEdit *m_n = _l.at(i);
    if (m_n->objectName().contains("payment/limits_"))
      m_n->updateCurrencySuffix(_cyrrency);
  }
}
