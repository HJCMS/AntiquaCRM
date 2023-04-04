// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "conditionedit.h"

#include <AntiquaCRM>

namespace AntiquaCRM {

ConditionEdit::ConditionEdit(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setToolTip(tr("Condition"));
  m_edit->setWithoutDisclosures(AntiquaCRM::Condition::NO_CONDITION);
  m_edit->addItem(tr("Very good, almost new!"), AntiquaCRM::Condition::FINE);
  m_edit->addItem(tr("Slight signs of wear."), AntiquaCRM::Condition::GOOD);
  m_edit->addItem(tr("Significant signs of use."),
                  AntiquaCRM::Condition::SATISFYING);
  m_edit->addItem(tr("Heavily worn!"), AntiquaCRM::Condition::SUFFICIENT);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void ConditionEdit::valueChanged(int index) {
  if (index == 0)
    return;

  setWindowModified(true);
  emit inputChanged();
}

void ConditionEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(AntiquaCRM::Condition::NO_CONDITION);
  setRestrictions(_f);
  setWindowModified(false);
}

void ConditionEdit::setValue(const QVariant &value) {
  AntiquaCRM::Condition _found = AntiquaCRM::Condition::NO_CONDITION;
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
    _found = static_cast<AntiquaCRM::Condition>(value.toInt());
    break;

  default:
    qWarning("Invalid given Data Type in ConditionEdit.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "ConditionEdit Requires type int but get:" << value;
#endif
    break;
  };

  for (int i = 0; i < m_edit->count(); i++) {
    int _v = m_edit->itemData(i, Qt::UserRole).toInt();
    AntiquaCRM::Condition _condition = static_cast<AntiquaCRM::Condition>(_v);
    if (_condition == _found) {
      m_edit->setCurrentIndex(i);
      break;
    }
  }
}

void ConditionEdit::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void ConditionEdit::reset() {
  setWindowModified(false);
  m_edit->setCurrentIndex(0);
}

void ConditionEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() != QSqlField::Required) {
    setRequired(false);
    return;
  }

  int _default = field.defaultValue().toInt();
  if (_default != getValue().toInt()) {
    setValue(_default);
  }
  setRequired(true);
}

void ConditionEdit::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void ConditionEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool ConditionEdit::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant ConditionEdit::getValue() {
  int _c = m_edit->itemData(m_edit->currentIndex(), Qt::UserRole).toInt();
  if (_c >= 0)
    return static_cast<AntiquaCRM::Condition>(_c);
  else
    return AntiquaCRM::Condition::NO_CONDITION;
}

const QString ConditionEdit::popUpHints() {
  QStringList info;
  info << tr("Missing Condition for this Article!");
  info << tr("The condition of an article should always be specified for the buyer.");
  return info.join("\n");
}

const QString ConditionEdit::statusHints() {
  return tr("No Condition for this Article exists!");
}

} // namespace AntiquaCRM
