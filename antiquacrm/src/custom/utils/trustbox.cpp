// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "trustbox.h"

#include <QDebug>
#include <QMap>
#include <QString>
#include <QHBoxLayout>

#include <AntiquaCRM>

TrustBox::TrustBox(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("TrustBox");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  layout->addWidget(m_info);

  m_trusted = new QComboBox(this);
  TrustList list;
  for (int i = 0; i < list.size(); i++)
  {
    Trust t = list.trust(i);
    m_trusted->insertItem(t.index(),t.title());
  }
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

void TrustBox::setFocus() { m_trusted->setFocus(); }

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
