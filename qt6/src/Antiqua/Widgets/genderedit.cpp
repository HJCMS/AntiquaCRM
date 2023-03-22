// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "genderedit.h"

namespace AntiquaCRM {

GenderEdit::GenderEdit(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setToolTip(tr("Gender"));
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void GenderEdit::valueChanged(int) { setWindowModified(true); }

void GenderEdit::initData() {
  m_edit->setWithoutDisclosures(AntiquaCRM::NO_GENDER);
  m_edit->insertItem(1, tr("Male"), AntiquaCRM::MALE);
  m_edit->setItemIcon(1, getIcon("user_male"));
  m_edit->insertItem(2, tr("Female"), AntiquaCRM::FEMALE);
  m_edit->setItemIcon(2, getIcon("user_female"));
  m_edit->insertItem(3, tr("Various"), AntiquaCRM::VARIOUS);
  m_edit->setItemIcon(3, getIcon("user_group"));
  m_edit->setCurrentIndex(0);
}

void GenderEdit::setValue(const QVariant &value) {
  int index = -1;
  int old = m_edit->currentIndex();

  switch (value.metaType().id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
  case (QMetaType::Double): {
    AntiquaCRM::Gender gen = static_cast<AntiquaCRM::Gender>(value.toInt());
    index = m_edit->findData(gen, Qt::UserRole);
  } break;

  case (QMetaType::QString): {
    index = m_edit->findData(value.toString(), Qt::DisplayRole);
  } break;

  default:
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("GenderEdit::setValue() Invalid Metatype.");
#endif
    return;
  };

  if (index >= 0)
    m_edit->setCurrentIndex(index);

  if (index != old)
    setWindowModified(true);
}

void GenderEdit::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void GenderEdit::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

const QHash<AntiquaCRM::Gender, QString> GenderEdit::genderData() {
  QString notset = AntiquaCRM::AComboBox::withoutDisclosures();
  QHash<AntiquaCRM::Gender, QString> h;
  h.insert(AntiquaCRM::NO_GENDER, notset);
  h.insert(AntiquaCRM::MALE, tr("Male"));
  h.insert(AntiquaCRM::FEMALE, tr("Female"));
  h.insert(AntiquaCRM::VARIOUS, tr("Various"));
  return h;
}

void GenderEdit::setRestrictions(const QSqlField &field) {
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

void GenderEdit::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void GenderEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  QLabel *m_lb = new QLabel(this);
  m_lb->setText(text + ":");
  m_lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_lb->setTextInteractionFlags(Qt::NoTextInteraction);
  m_lb->setBuddy(m_edit);
  layout->insertWidget(0, m_lb);
}

bool GenderEdit::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant GenderEdit::getValue() {
  int _g = m_edit->itemData(m_edit->currentIndex(), Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::Gender>(_g);
}

const QString GenderEdit::popUpHints() { return tr(""); }

const QString GenderEdit::statusHints() { return tr("Gender not selected!"); }

} // namespace AntiquaCRM