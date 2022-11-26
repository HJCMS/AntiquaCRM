// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "trustedbox.h"

TrustedBox::TrustedBox(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Trust"));
  m_box->setWhatsThis(tr("Changes the Trustlevel for this customer."));
  m_layout->addWidget(m_box);
  loadDataset();
  setRequired(true);
  setModified(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void TrustedBox::loadDataset() {
  // Keine Vertrauensebene festgelegt
  m_box->insertItem(0, tr("No trust level set"), AntiquaCRM::NO_TRUST_LEVEL);
  // Zahlt Pünktlich
  m_box->insertItem(0, tr("Pays on time"), AntiquaCRM::ON_TIME);
  // Zahlt Zuverlässig
  m_box->insertItem(0, tr("Pays reliably"), AntiquaCRM::RELIABLE);
  // Zahlt mit Verspätung
  m_box->insertItem(0, tr("Pay late"), AntiquaCRM::WITH_DELAY);
  // Nur mit Vorauszahlung
  m_box->insertItem(0, tr("Only with prepayment"), AntiquaCRM::PREPAYMENT);
  // Keine Lieferung
  m_box->insertItem(0, tr("No delivery"), AntiquaCRM::NO_DELIVERY);
}

void TrustedBox::dataChanged(int) { setModified(true); }

void TrustedBox::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void TrustedBox::setValue(const QVariant &val) {
  AntiquaCRM::CustomerTrustLevel tl =
      static_cast<AntiquaCRM::CustomerTrustLevel>(val.toInt());

  int index = m_box->findData(tl, Qt::UserRole, Qt::MatchExactly);
  if (index > 0)
    m_box->setCurrentIndex(index);
}

void TrustedBox::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void TrustedBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant TrustedBox::value() {
  int i = m_box->currentIndex();
  return static_cast<AntiquaCRM::CustomerTrustLevel>(
      m_box->itemData(i, Qt::UserRole).toInt());
}

bool TrustedBox::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void TrustedBox::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString TrustedBox::info() { return m_box->toolTip(); }

const QString TrustedBox::notes() { return tr("a Trustlevel is required."); }
