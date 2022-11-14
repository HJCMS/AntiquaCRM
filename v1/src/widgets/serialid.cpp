// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "serialid.h"

#include <QDebug>
#include <QHBoxLayout>

SerialID::SerialID(QWidget *parent) : InputEdit{parent} {
  setObjectName("SerialID");

  m_serialLabel = new QLabel(this);
  m_serialLabel->setTextFormat(Qt::PlainText);
  m_serialLabel->setStyleSheet("font-weight: bold;");
  m_serialLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                                         Qt::TextSelectableByMouse);
  m_layout->addWidget(m_serialLabel);

  setModified(false);
  setRequired(true);

  connect(this, SIGNAL(sendRequireChanged()), SLOT(loadDataset()));
}

void SerialID::loadDataset() {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "Required Changed:" << objectName() << isRequired();
#endif
}

void SerialID::setValue(const QVariant &id) {
  m_serialLabel->setText(id.toString());
  emit sendSerialChanged(id.toInt());
  setModified(true);
}

void SerialID::reset() {
  m_serialLabel->clear();
  setModified(false);
}

void SerialID::setFocus() { m_serialLabel->setFocus(); }

void SerialID::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  setToolTip(info);
}

const QString SerialID::info() { return m_label->text(); }

void SerialID::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant SerialID::value() {
  QVariant data = QVariant(m_serialLabel->text()).toULongLong();
  /**< @warning Es darf keine 0 zurück gegeben werden */
  if (data.toInt() < 1)
    return QVariant();

  return data;
}

bool SerialID::isValid() {
  if (isRequired() && m_serialLabel->text().isEmpty())
    return false;

  QVariant data(m_serialLabel->text().trimmed());
  if (data.toULongLong() >= 0)
    return true;

  return false;
}

const QString SerialID::notes() {
  QString msg("'" + info() + "' ");
  msg.append(tr("is required and can not empty."));
  return msg;
}
