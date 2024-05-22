// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodelocation.h"
#include "postalcodeedit.h"

namespace AntiquaCRM
{

PostalCodeLocation::PostalCodeLocation(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new ALineEdit(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(editingFinished()), SLOT(updateChanged()));
}

void PostalCodeLocation::initData() {
  // psql -c "\d customers" | grep c_location
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  setRestrictions(_f);
  setWindowModified(false);
}

void PostalCodeLocation::updateChanged() {
  if (!isValid())
    return;

  if (p_history.compare(m_edit->text(), Qt::CaseSensitive) != 0) {
    setWindowModified(true);
    emit sendInputChanged();
  }
}

void PostalCodeLocation::setValue(const QVariant& value) {
  QString _location;
  if (value.metaType().id() == QMetaType::QString) {
    _location = value.toString().trimmed();
  } else {
    qWarning("Invalid location input!");
    return;
  }
  p_history = _location;
  m_edit->setText(_location);
}

void PostalCodeLocation::setFocus() {
  m_edit->setFocus();
}

void PostalCodeLocation::reset() {
  p_history.clear();
  m_edit->clear();
  m_edit->setCompleter(nullptr);
  m_edit->setCompleterAction(false);
  setWindowModified(false);
}

bool PostalCodeLocation::setCompletion(AntiquaCRM::PostalCodeEdit* obj,
                                       const AntiquaCRM::PostalCode& code) {
  if (code.location.isEmpty())
    return false;

  if (obj == nullptr)
    return false;

  // WARNING - do not override it!
  QString _str = m_edit->text();
  if (_str.isEmpty())
    m_edit->setText(code.location);

  QCompleter* m_cpl = obj->getCompleter(m_edit);
  if (m_cpl == nullptr)
    return false;

  m_edit->setClearButtonEnabled(true);
  m_edit->setCompleter(m_cpl);
  m_edit->setCompleterAction(true);
  return true;
}

void PostalCodeLocation::setRestrictions(const QSqlField& field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.metaType().id() != QMetaType::QString)
    return;

  m_edit->setLineEditProperties(field);
}

bool PostalCodeLocation::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

void PostalCodeLocation::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void PostalCodeLocation::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

const QMetaType PostalCodeLocation::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant PostalCodeLocation::getValue() {
  return m_edit->text().trimmed();
}

const QString PostalCodeLocation::popUpHints() {
  QStringList _l(tr("Missing location in this dataset!"));
  _l << tr("A Postal location is very important to write valid invoices.");
  return _l.join("<br>");
}

const QString PostalCodeLocation::statusHints() {
  return tr("Missing a valid Postal location!");
}

} // namespace AntiquaCRM
