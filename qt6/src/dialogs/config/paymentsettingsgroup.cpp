// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsettingsgroup.h"

#include <QLayout>

PaymentSettingsGroup::PaymentSettingsGroup(QWidget *parent)
    : QGroupBox{parent} {
  setTitle(tr("Payment settings"));

  qint8 row = 0;
  QGridLayout *g1Layout = new QGridLayout(this);
  g1Layout->setContentsMargins(5, 5, 5, 5);
  m_currency = new AntiquaCRM::SelectCurrency(this);
  m_currency->setObjectName("payment/currency");
  m_currency->setBuddyLabel(tr("Currency"));
  m_currency->appendStretch(1);
  g1Layout->addWidget(m_currency, row++, 0, 1, 2);

  m_vatNormal = new AntiquaCRM::NumEdit(this);
  m_vatNormal->setObjectName("payment/vat_normal");
  m_vatNormal->setBuddyLabel(tr("VAT (normal)"));
  m_vatNormal->setSuffix("%");
  g1Layout->addWidget(m_vatNormal, row, 0, 1, 1);

  m_vatReduced = new AntiquaCRM::NumEdit(this);
  m_vatReduced->setObjectName("payment/vat_reduced");
  m_vatReduced->setBuddyLabel(tr("VAT (reduced)"));
  m_vatReduced->setSuffix("%");
  m_vatReduced->appendStretch(1);
  g1Layout->addWidget(m_vatReduced, row++, 1, 1, 1);

  g1Layout->setRowStretch(row++, 1);
  setLayout(g1Layout);
}
