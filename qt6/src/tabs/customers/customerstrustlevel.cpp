// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstrustlevel.h"

CustomersTrustLevel::CustomersTrustLevel(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setToolTip(tr("Trust"));
  m_edit->setWhatsThis(tr("Changes the Trustlevel for this customer."));
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void CustomersTrustLevel::valueChanged(int) { setWindowModified(true); }

void CustomersTrustLevel::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setDefaultValue(0);

  QIcon green = AntiquaCRM::antiquaIcon("flag-green");
  QIcon yellow = AntiquaCRM::antiquaIcon("flag-yellow");
  QIcon red = AntiquaCRM::antiquaIcon("flag-red");
  // Keine Vertrauensebene festgelegt
  m_edit->setWithoutDisclosures(AntiquaCRM::CustomerTrustLevel::NO_TRUST_LEVEL);
  // Zahlt Pünktlich
  m_edit->addItem(green, tr("Pays on time"),
                  AntiquaCRM::CustomerTrustLevel::ON_TIME);
  // Zahlt Zuverlässig
  m_edit->addItem(green, tr("Pays reliably"),
                  AntiquaCRM::CustomerTrustLevel::RELIABLE);
  // Zahlt mit Verspätung
  m_edit->addItem(yellow, tr("Pay late"),
                  AntiquaCRM::CustomerTrustLevel::WITH_DELAY);
  // Nur mit Vorauszahlung
  m_edit->addItem(yellow, tr("Only with prepayment"),
                  AntiquaCRM::CustomerTrustLevel::PREPAYMENT);
  // Keine Lieferung
  m_edit->addItem(red, tr("No delivery"),
                  AntiquaCRM::CustomerTrustLevel::NO_DELIVERY);

  setRestrictions(_f);
  setWindowModified(false);
}

void CustomersTrustLevel::setValue(const QVariant &value) {
  int _index = -1;
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
    _index = m_edit->findData(value.toInt(), Qt::UserRole);
    break;

  default:
    qWarning("Invalid given Data Type in CustomersTrustLevel.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "CustomersTrustLevel Requires type int but get:" << value;
#endif
    break;
  };

  if (_index != -1)
    m_edit->setCurrentIndex(_index);
}

void CustomersTrustLevel::setFocus() { m_edit->setFocus(); }

void CustomersTrustLevel::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void CustomersTrustLevel::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void CustomersTrustLevel::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void CustomersTrustLevel::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  AntiquaCRM::ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool CustomersTrustLevel::isValid() {
  if (isRequired() && m_edit->currentIndex() < 1)
    return false;

  return true;
}

const QMetaType CustomersTrustLevel::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant CustomersTrustLevel::getValue() {
  int _index = m_edit->currentIndex();
  return static_cast<AntiquaCRM::CustomerTrustLevel>(
      m_edit->itemData(_index, Qt::UserRole).toInt());
}

const QString CustomersTrustLevel::popUpHints() {
  return tr("A Trustlevel is required.");
}

const QString CustomersTrustLevel::statusHints() { return popUpHints(); }
