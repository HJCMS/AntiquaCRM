// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservice.h"
#include "deliverpackagebox.h"
#include "deliverservicebox.h"

DeliverService::DeliverService(QWidget *parent) : InputEdit{parent} {
  setObjectName("delivery_service_edit");
  m_cfg = new AntiquaCRM::ASettings(this);
  p_currency = m_cfg->value("payment/currency", "$").toString();

  m_serviceBox = new DeliverServiceBox(this);
  m_serviceBox->insertItem(0, tr("Internal"));
  m_layout->insertWidget(1, m_serviceBox);
  m_packageBox = new DeliverPackageBox(this);
  m_packageBox->insertItem(1, tr("Internal"));
  m_packageBox->setEnabled(false);
  m_layout->insertWidget(2, m_packageBox);
  m_priceInfo = new QLabel(this);
  m_priceInfo->setMinimumWidth(30);
  m_priceInfo->setMaximumWidth(50);
  m_layout->insertWidget(3, m_priceInfo);
  m_layout->setStretch(2, 1);
  setRequired(false);

  connect(m_serviceBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(currentServiceChanged(int)));

  connect(m_packageBox, SIGNAL(validServiceChanged(bool)), this,
          SLOT(getPriceOnDemand(bool)));

  connect(m_packageBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(packageChanged(int)));
}

void DeliverService::getPriceOnDemand(bool b) {
  Q_UNUSED(b);
  m_packageBox->setEnabled(true);
}

void DeliverService::currentServiceChanged(int) {
  int did = m_serviceBox->getCurrentServiceId();
  m_packageBox->setCurrentPackages(did);
  emit sendDeliveryService(did);
}

void DeliverService::packageChanged(int) {
  int cid = m_packageBox->getCurrentPackageId();
  if (cid > 0) {
    qreal price = m_packageBox->getPackagePrice(cid);
    QString ptxt = QString::number(price, 'f', 2);
    m_priceInfo->setText(ptxt + " " + p_currency);
    setModified(true);
    emit sendDeliveryPackage(cid);
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
  m_priceInfo->clear();
  setModified(false);
}

void DeliverService::setFocus() { m_packageBox->setFocus(); }

void DeliverService::setProperties(const QSqlField &field) {
  Q_UNUSED(field);
}

void DeliverService::loadDataset() { m_serviceBox->initDeliverServices(); }

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
  return tr("Delivery Service is needed!");
}

void DeliverService::setDeliveryService(const QPair<int, int> &pair) {
  reset();
  blockSignals(true);
  setDeliveryService(pair.first);
  setDeliveryPackage(pair.second);
  blockSignals(false);

  qreal price = m_packageBox->getPackagePrice(pair.second);
  m_priceInfo->setText(QString::number(price, 'f', 2) + " " + p_currency);
}

const QPair<int, int> DeliverService::currentDeliveryService() {
  QPair<int, int> service(0, 0);
  service.first = value().toInt();
  service.first = getDeliveryPackage();
  return service;
}

const QPair<int, int> DeliverService::defaultDeliveryService() {
  // QPair<Service,Paket>
  QPair<int, int> service(0, 0);
  // 1) Sind die Parameter bereits in der Konfiguration vorhanden?
  if (m_cfg->contains("delivery/service") &&
      m_cfg->contains("delivery/package")) {
    service.first = m_cfg->value("delivery/service").toInt();
    service.second = m_cfg->value("delivery/package").toInt();
    if (service.first != 0 && service.second != 0)
      return service;
  }
  // 2) Aus Datenbank nehmen und in Konfiguration speichern!
  AntiquaCRM::ASqlCore querySql(this);
  QSqlQuery q = querySql.query(
      AntiquaCRM::ASqlFiles::queryStatement("query_default_delivery_service"));
  if (q.size() > 0) {
    q.next();
    // Service Nummer
    service.first = q.value("d_srv").toInt();
    m_cfg->setValue("delivery/service", service.first);
    // Kosten Nummer
    service.second = q.value("d_cid").toInt();
    m_cfg->setValue("delivery/package", service.second);
  } else {
    qWarning("Can't not fetch Default Delivery Servive!");
  }
  return service;
}
