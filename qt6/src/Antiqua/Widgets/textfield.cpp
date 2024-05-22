// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

namespace AntiquaCRM
{

TextField::TextField(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  layout->setDirection(QBoxLayout::TopToBottom);

  m_edit = new ATextEdit(this);
  layout->addWidget(m_edit);

  connect(m_edit, SIGNAL(textChanged()), SLOT(valueChanged()));
  initData();
}

void TextField::valueChanged() {
  setWindowModified(true);
  emit sendInputChanged();
}

void TextField::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
}

void TextField::setValue(const QVariant& value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_edit->setText(value.toString());
    return;
  }

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << "TextField Requires QString but get:" << objectName() << value;
#endif
}

void TextField::setFocus() {
  m_edit->setFocus();
}

void TextField::reset() {
  m_edit->setText(QString());
  m_edit->clear();
  setWindowModified(false);
}

void TextField::setRestrictions(const QSqlField& field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void TextField::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void TextField::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
  m_lb->setAlignment(Qt::AlignLeft);
}

bool TextField::isValid() {
  const QString _s = getValue().toString();
  if (isRequired() && (_s.length() < 1))
    return false;

  return true;
}

const QMetaType TextField::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant TextField::getValue() {
  return m_edit->text().trimmed();
}

const QString TextField::popUpHints() {
  return tr("This Text field requires a valid input.");
}

const QString TextField::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
