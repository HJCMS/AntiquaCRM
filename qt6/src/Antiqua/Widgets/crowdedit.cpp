// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "crowdedit.h"

namespace AntiquaCRM {

CrowdEdit::CrowdEdit(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ASpinBox(this);
  m_edit->setMinimum(0);
  m_edit->setMaximum(999);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void CrowdEdit::valueChanged(int) {
  if (isValid())
    setWindowModified(true);
}

void CrowdEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(0);
  if (!objectName().isEmpty())
    _f.setName(objectName());

  setRestrictions(_f);
  setWindowModified(false);
}

void CrowdEdit::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
    m_edit->setValue(value.toInt());
    break;

  default:
    qWarning("Invalid given Data Type in CrowdEdit.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "CrowdEdit Requires type int but get:" << value;
#endif
    break;
  };
}

void CrowdEdit::setFocus() { m_edit->setFocus(); }

void CrowdEdit::reset() {
  setWindowModified(false);
  m_edit->setValue(m_edit->minimum());
}

void CrowdEdit::setRestrictions(const QSqlField &) {}

void CrowdEdit::setInputToolTip(const QString &str) { m_edit->setToolTip(str); }

void CrowdEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  QLabel *m_lb = new QLabel(this);
  m_lb->setText(text + ":");
  m_lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_lb->setTextInteractionFlags(Qt::NoTextInteraction);
  m_lb->setBuddy(m_edit);
  layout->insertWidget(0, m_lb);
}

bool CrowdEdit::isValid() {
  if (isRequired() && m_edit->value() < 0)
    return false;

  return true;
}

const QVariant CrowdEdit::getValue() { return m_edit->value(); }

const QString CrowdEdit::popUpHints() {
  return tr("This entry requires a valid article count!");
}

const QString CrowdEdit::statusHints() {
  return tr("Article count is to low!");
}

} // namespace AntiquaCRM
