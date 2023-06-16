// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodestate.h"

#include <QDebug>

namespace AntiquaCRM {

PostalCodeState::PostalCodeState(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new ALineEdit(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(editingFinished()), SLOT(updateChanged()));
}

void PostalCodeState::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(100);
  setRestrictions(_f);
  setWindowModified(false);
}

void PostalCodeState::updateChanged() {
  if (!isValid())
    return;

  if (p_history.compare(m_edit->text(), Qt::CaseSensitive) != 0) {
    setWindowModified(true);
    emit sendInputChanged();
  }
}

void PostalCodeState::setCountry(const AntiquaCRM::PostalCode &code) {
  if (code.state.isEmpty())
    return;

  m_edit->setText(code.state);
}

void PostalCodeState::setValue(const QVariant &value) {
  QString _country;
  if (value.metaType().id() == QMetaType::QString) {
    _country = value.toString().trimmed();
  } else {
    qWarning("Invalid country input!");
    return;
  }
  p_history = _country;
  m_edit->setText(_country);
}

void PostalCodeState::setFocus() { m_edit->setFocus(); }

void PostalCodeState::reset() {
  p_history.clear();
  m_edit->clear();
  setWindowModified(false);
}

void PostalCodeState::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.metaType().id() != getType().id())
    return;

  m_edit->setLineEditProperties(field);
}

bool PostalCodeState::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

void PostalCodeState::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void PostalCodeState::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

const QMetaType PostalCodeState::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant PostalCodeState::getValue() { return m_edit->text().trimmed(); }

const QString PostalCodeState::popUpHints() {
  QStringList _l(tr("Missing Country/State in this dataset!"));
  _l << tr("Country/State is very important to write valid invoices.");
  return _l.join("<br>");
}

const QString PostalCodeState::statusHints() {
  return tr("Missing Country/State for Postalcode!");
}

} // namespace AntiquaCRM
