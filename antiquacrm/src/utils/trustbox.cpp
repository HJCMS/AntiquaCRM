// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "trustbox.h"

#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtWidgets/QHBoxLayout>

TrustBox::TrustBox(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("TrustBox");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  layout->addWidget(m_info);

  m_trusted = new QComboBox(this);
  m_trusted->insertItem(0, tr("Trust Settings"));
  m_trusted->insertItem(1, tr("Pays on time"));
  m_trusted->insertItem(2, tr("Pays reliable"));
  m_trusted->insertItem(3, tr("Pays with delay"));
  m_trusted->insertItem(4, tr("Prepayment"));
  m_trusted->insertItem(5, tr("Has been warned before"));
  m_trusted->insertItem(6, tr("Already had a debt collection process"));
  m_trusted->insertItem(7, tr("Collection procedure unsuccessful"));
  m_trusted->insertItem(8, tr("No delivery or order"));
  layout->addWidget(m_trusted);

  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_trusted, SIGNAL(currentIndexChanged(int)), this,
          SLOT(valueChanged(int)));
}

void TrustBox::valueChanged(int) { setModified(true); }

void TrustBox::setValue(const QVariant &val) {
  int index = val.toInt();
  if (index > 0)
    m_trusted->setCurrentIndex(index);
}

void TrustBox::reset() {
  m_trusted->setCurrentIndex(0);
  setModified(false);
}

const QVariant TrustBox::value() { return m_trusted->currentIndex(); }

bool TrustBox::isValid() { return true; }

void TrustBox::setInfo(const QString &info) {
  return m_info->setToolTip(info);
  return m_info->setText(info);
}

const QString TrustBox::info() { return m_info->text(); }

const QString TrustBox::notes() {
  return tr("Trusted Verification is required.");
}
