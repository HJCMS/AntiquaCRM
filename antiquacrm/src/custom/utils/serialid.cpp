// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "serialid.h"
#include "antiqua_global.h"

#include <QDebug>
#include <QHBoxLayout>

SerialID::SerialID(QWidget *parent) : UtilsMain{parent} {
  setObjectName("SerialID");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_infoLabel = new QLabel(this);
  m_infoLabel->setAlignment(labelAlignment());
  layout->addWidget(m_infoLabel);

  m_serialLabel = new QLabel(this);
  m_serialLabel->setTextFormat(Qt::PlainText);
  m_serialLabel->setStyleSheet("font-weight: bold;");
  m_serialLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                                         Qt::TextSelectableByMouse);
  layout->addWidget(m_serialLabel);

  setModified(false);
  setLayout(layout);
}

void SerialID::setValue(const QVariant &id) {
  m_serialLabel->setText(id.toString());
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
  m_infoLabel->setText(txt);
  m_serialLabel->setToolTip(info);
}

const QString SerialID::info() { return m_infoLabel->text(); }

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

  if (QVariant(m_serialLabel->text()).toULongLong() < 1)
    return false;

  return true;
}

const QString SerialID::notes() {
  return tr("The Arcticle ID is required and can not empty.");
}
