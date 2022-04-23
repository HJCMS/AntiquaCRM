// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "serialid.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

SerialID::SerialID(QWidget *parent) : UtilsMain{parent} {
  setObjectName("SerialID");
  setWindowTitle(tr("Article ID"));
  setToolTip(tr("Article ID"));
  setContentsMargins(0, 0, 0, 0);
  setModified(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_infoLabel = new QLabel(this);
  m_infoLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
  m_infoLabel->setText(tr("Article ID:"));
  layout->addWidget(m_infoLabel);

  m_serialLabel = new QLabel(this);
  m_serialLabel->setTextFormat(Qt::PlainText);
  m_serialLabel->setStyleSheet("font-weight: bold;");
  m_serialLabel->setAlignment(Qt::AlignLeft | Qt::AlignTrailing | Qt::AlignVCenter);
  m_serialLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                                    Qt::TextSelectableByMouse);
  layout->addWidget(m_serialLabel);
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

bool SerialID::hasModified() { return isModified(); }

void SerialID::setInfo(const QString &txt) { m_infoLabel->setText(txt); }

const QString SerialID::info() { return m_infoLabel->text(); }

const QVariant SerialID::value() {
  QVariant data = QVariant(m_serialLabel->text()).toULongLong();
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
