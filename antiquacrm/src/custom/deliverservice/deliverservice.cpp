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

  ApplSettings cfg;
  p_currency = cfg.value("payment/currency", "$").toString();

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_serviceBox = new DeliverServiceBox(this);
  m_serviceBox->insertItem(0, tr("Internal"));
  layout->addWidget(m_serviceBox);

  m_packageBox = new DeliverPackageBox(this);
  m_packageBox->insertItem(1, tr("Internal"));
  m_packageBox->setMinimumWidth(100);
  m_packageBox->setEnabled(false);
  layout->addWidget(m_packageBox);

  m_priceInfo = new QLabel(this);
  layout->addWidget(m_priceInfo);

  layout->addStretch(1);
  setLayout(layout);
  setRequired(false);

  connect(m_serviceBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(currentServiceChanged(int)));

  connect(m_packageBox, SIGNAL(validServiceChanged(bool)), this,
          SLOT(getPriceOnDemand(bool)));

  connect(m_packageBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(packageChanged(int)));
}

void DeliverService::getPriceOnDemand(bool b) {
  m_packageBox->setEnabled(true);
  // FIXME if(b) { packageChanged(1); }
}

void DeliverService::currentServiceChanged(int) {
  int did = m_serviceBox->getCurrentServiceId();
  m_packageBox->setCurrentPackages(did);
}

void DeliverService::packageChanged(int) {
  int cid = m_packageBox->getCurrentPackageId();
  if (cid > 0) {
    qreal price = m_packageBox->getPackagePrice(cid);
    QString ptxt = QString::number(price, 'f', 2);
    m_priceInfo->setText(ptxt + " " + p_currency);
    setModified(true);
  }
}

void DeliverService::setValue(const QVariant &val) {
  int index = -1;
  if (val.type() == QVariant::Int) {
    index = m_serviceBox->findData(val.toInt(), Qt::UserRole);
  } else {
    index = m_serviceBox->findData(val.toString(), Qt::DisplayRole);
  }
  if (index > 0) {
    m_serviceBox->setCurrentIndex(index);
    setModified(true);
  }
}

void DeliverService::reset() {
  m_serviceBox->setCurrentIndex(0);
  m_packageBox->setCurrentIndex(0);
  setModified(false);
}

void DeliverService::setFocus() { m_packageBox->setFocus(); }

void DeliverService::loadSqlDataset() { m_serviceBox->initDeliverServices(); }

const QVariant DeliverService::value() {
  return m_serviceBox->getCurrentServiceId();
}

void DeliverService::setDeliveryService(int did) {
  m_serviceBox->setCurrentServiceId(did);
}

void DeliverService::setDeliveryPackage(int cid) {
  return m_packageBox->setCurrentPackageId(cid);
}

int DeliverService::getDeliveryPackage() {
  return m_packageBox->getCurrentPackageId();
}

bool DeliverService::isInternational() {
  // Ist es ein Nationales oder Internationales
  return m_packageBox->isInternational();
}

qreal DeliverService::getPackagePrice() {
  int cid = m_packageBox->getCurrentPackageId();
  return (cid > 0) ? m_packageBox->getPackagePrice(cid) : 0.00;
}

bool DeliverService::isValid() { return true; }

void DeliverService::setInfo(const QString &info) { setToolTip(info); }

const QString DeliverService::info() { return toolTip(); }

const QString DeliverService::notes() {
  return tr("Delevery Service is needet!");
}
