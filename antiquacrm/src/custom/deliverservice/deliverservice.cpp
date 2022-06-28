// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservice.h"
#include "applsettings.h"
#include "deliverpackagebox.h"
#include "deliverservicebox.h"

#include <QDebug>
#include <QHBoxLayout>

DeliverService::DeliverService(QWidget *parent) : UtilsMain{parent} {
  setObjectName("delivery_service_edit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_serviceBox = new DeliverServiceBox(this);
  m_serviceBox->insertItem(0, tr("Internal"));
  layout->addWidget(m_serviceBox);

  m_packageBox = new DeliverPackageBox(this);
  m_packageBox->insertItem(1, tr("Internal"));
  m_packageBox->setEnabled(false);
  layout->addWidget(m_packageBox);

  m_priceInfo = new QLabel(this);
  layout->addWidget(m_priceInfo);

  setLayout(layout);
  setRequired(false);

  connect(m_serviceBox, SIGNAL(currentIndexChanged(int)), m_packageBox,
          SLOT(setCurrentService(int)));

  connect(m_packageBox, SIGNAL(validServiceChanged(bool)), m_packageBox,
          SLOT(setEnabled(bool)));

  connect(m_packageBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(packageChanged(int)));
}

void DeliverService::packageChanged(int id) {
  qDebug() << Q_FUNC_INFO << id;
  setModified(true);
}

void DeliverService::setValue(const QVariant &val) {
  int i = -1;
  if (val.type() == QVariant::Int) {
    i = val.toInt();
  } else {
    i = m_serviceBox->findData(val.toString());
  }

  if (i > 0) {
    m_serviceBox->setCurrentIndex(i);
    setModified(true);
  }
}

void DeliverService::reset() {
  m_serviceBox->setCurrentIndex(0);
  setModified(false);
}

void DeliverService::setFocus() { m_serviceBox->setFocus(); }

void DeliverService::loadSqlDataset() { m_serviceBox->setDeliverServices(); }

const QVariant DeliverService::value() { return m_packageBox->currentIndex(); }

bool DeliverService::isValid() {
  if (isRequired() && m_packageBox->currentIndex() == 0)
    return false;

  return true;
}

void DeliverService::setInfo(const QString &info) {
  setToolTip(info);
}

const QString DeliverService::info() { return toolTip(); }

const QString DeliverService::notes() {
  return tr("Delevery Service is needet!");
}
