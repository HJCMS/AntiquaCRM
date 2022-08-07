// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cancellation.h"

#include <QHBoxLayout>

Cancellation::Cancellation(QWidget *parent, Cancellation::Mode mode)
    : UtilsMain{parent}, p_mode{mode} {

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_box = new QComboBox(this);
  m_box->insertItem(0, tr("without disclosures"));
  m_box->insertItem(1, tr("is currently not available"));
  m_box->insertItem(2, tr("item is sold out"));
  m_box->insertItem(3, tr("the client has canceled"));
  m_box->insertItem(4, tr("No delivery address available"));
  m_box->insertItem(5, tr("Creditworthiness"));
  layout->addWidget(m_box);

  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void Cancellation::itemChanged(int) { setModified(true); }

void Cancellation::setValue(const QVariant &val) {
  int i = 0;
  if (p_mode == Cancellation::TEXT) {
    i = m_box->findData(val.toString(), Qt::UserRole);
  } else {
    i = val.toInt();
  }

  if (i > 0) {
    p_value = i;
    m_box->setCurrentIndex(i);
    setModified(true);
  }
}

void Cancellation::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void Cancellation::setFocus() { m_box->setFocus(); }

const QVariant Cancellation::value() {
  if (p_mode == Cancellation::TEXT) {
    return m_box->currentText();
  } else {
    return m_box->currentIndex();
  }
}

bool Cancellation::isValid() { return true; }

void Cancellation::setInfo(const QString &info) {
  m_info->setText(info);
  m_box->setToolTip(info);
}

const QString Cancellation::info() { return m_box->toolTip(); }

const QString Cancellation::notes() { return tr("a cancellation is required"); }
