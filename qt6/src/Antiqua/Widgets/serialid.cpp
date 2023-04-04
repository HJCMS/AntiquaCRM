// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "serialid.h"

#include <QDebug>

namespace AntiquaCRM {

SerialId::SerialId(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setReadOnly(true);
  m_edit->setStyleSheet("QLineEdit {border:none;background:transparent;}");
  m_edit->setClearButtonEnabled(false);
  layout->addWidget(m_edit);
  initData();
}

void SerialId::valueChanged() { setWindowModified(true); }

void SerialId::initData() { setWindowModified(false); }

void SerialId::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
    m_edit->setText(QString::number(value.toInt()));
    break;

  default:
    qWarning("Invalid given Data Type in SerialId.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "SerialId Requires type int but get:" << value;
#endif
    m_edit->setText(value.toString());
    break;
  };
}

void SerialId::setFocus() { m_edit->setFocus(); }

void SerialId::reset() {
  setWindowModified(false);
  m_edit->clear();
}

void SerialId::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void SerialId::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void SerialId::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SerialId::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  QVariant data(m_edit->text().trimmed());
  qint64 isbn = data.toLongLong();
  if (isbn >= 0)
    return true;

  return false;
}

const QVariant SerialId::getValue() {
  quint64 _value = QVariant(m_edit->text()).toLongLong();
  /**< @warning Es darf keine 0 zurück gegeben werden */
  if (_value < 1)
    return QVariant();

  return _value;
}

const QString SerialId::popUpHints() {
  return tr("A valid Serial Id is important!");
}

const QString SerialId::statusHints() {
  return tr("is required and can not empty.");
}

} // namespace AntiquaCRM
