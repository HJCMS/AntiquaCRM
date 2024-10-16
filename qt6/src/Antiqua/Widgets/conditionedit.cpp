// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "conditionedit.h"

#include <AntiquaCRM>

namespace AntiquaCRM
{

ConditionEdit::ConditionEdit(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setToolTip(tr("Condition"));
  m_edit->setWithoutDisclosures(AntiquaCRM::Condition::NO_CONDITION);
  m_edit->addItem(tr("Very good, almost new!"), AntiquaCRM::Condition::FINE);
  m_edit->addItem(tr("Slight signs of wear."), AntiquaCRM::Condition::GOOD);
  m_edit->addItem(tr("Significant signs of use."), AntiquaCRM::Condition::SATISFYING);
  m_edit->addItem(tr("Heavily worn!"), AntiquaCRM::Condition::SUFFICIENT);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void ConditionEdit::valueChanged(int index) {
  if (index == 0)
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void ConditionEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(AntiquaCRM::Condition::NO_CONDITION);
  setRestrictions(_f);
  setWindowModified(false);
}

void ConditionEdit::setValue(const QVariant& value) {
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
#ifdef ANTIQUA_DEVELOPMENT
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
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

const QString ConditionEdit::defaultWhatsThis() const {
  QString _info = tr("<div>Article condition is in following rating available.</div>");
  _info.append("<ol>");
  for (int i = 0; i < m_edit->count(); i++) {
    QString _txt = m_edit->itemText(i);
    _info.append("<li>");
    switch (m_edit->itemData(i, Qt::UserRole).toInt()) {
      case (AntiquaCRM::Condition::FINE):
        _info.append(tr("Fine") + ": " + _txt);
        break;

      case (AntiquaCRM::Condition::GOOD):
        _info.append(tr("Good") + ": " + _txt);
        break;

      case (AntiquaCRM::Condition::SATISFYING):
        _info.append(tr("Satisfying") + ": " + _txt);
        break;

      case (AntiquaCRM::Condition::SUFFICIENT):
        _info.append(tr("Sufficient") + ": " + _txt);
        break;

      default:
        _info.append(tr("Unknown") + ": " + _txt);
    }
    _info.append("</li>");
  }
  _info.append("</ol>");
  return _info;
}

void ConditionEdit::setRestrictions(const QSqlField&) {
  setRequired(true);
}

void ConditionEdit::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void ConditionEdit::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool ConditionEdit::isValid() {
  if (isRequired() && m_edit->currentIndex() < 1)
    return false;

  return true;
}

const QMetaType ConditionEdit::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant ConditionEdit::getValue() {
  int _c = m_edit->itemData(m_edit->currentIndex(), Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::Condition>(_c);
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
