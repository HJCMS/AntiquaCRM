// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliveryserviceplus.h"
#include "applsettings.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLocale>

static const QString strCountry() {
  QLocale lc = QLocale::system();
  return lc.countryToString(lc.country());
}

DeliveryServicePlus::DeliveryServicePlus(QWidget *parent) : UtilsMain{parent} {
  setObjectName("delivery_service_plus");

  m_sql = new HJCMS::SqlCore(this);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_serviceBox = new QComboBox(this);
  m_serviceBox->setObjectName("services");
  m_serviceBox->insertItem(0, tr("Internal"));
  layout->addWidget(m_serviceBox);

  m_packageBox = new QComboBox(this);
  m_packageBox->setObjectName("packages");
  m_packageBox->insertItem(1, tr("Package"));
  m_packageBox->setEnabled(false);
  layout->addWidget(m_packageBox);

  m_price = new QLabel(this);
  layout->addWidget(m_price);

  setLayout(layout);

  setRequired(false);

  connect(m_serviceBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(changedService(int)));
  connect(m_packageBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(changedPackage(int)));
}

void DeliveryServicePlus::loadSqlPackage(int id) {
  m_packageBox->clear();
  if (id < 1) {
    m_packageBox->setEnabled(false);
    return;
  }

  QString sql("SELECT * FROM ref_delivery_cost");
  sql.append(" WHERE d_srv=");
  sql.append(QString::number(id));
  sql.append(" ORDER BY d_cid;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int i = q.value("d_cid").toInt();
      QString t = q.value("d_class").toString();
      t.append(" ");
      t.append(q.value("d_definition").toString());
      bool b = q.value("d_international").toBool();
      if (b) {
        t.append(" (" + strCountry() + ") ");
      } else {
        t.append(" (International)");
      }
      m_packageBox->insertItem(i, t);
    }
    m_packageBox->setEnabled(true);
  }
}

void DeliveryServicePlus::changedService(int i) {
  p_service = i;
  loadSqlPackage(i);
  setModified(true);
}

void DeliveryServicePlus::changedPackage(int i) {
  p_package = i;
  ApplSettings cfg;
  QString currency = cfg.value("payment/currency", "€").toString();
  QString sql("SELECT d_price FROM ref_delivery_cost");
  sql.append(" WHERE d_cid=");
  sql.append(QString::number(i));
  sql.append(";");
  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    qreal price = q.value("d_price").toDouble();
    m_price->setText(QString::number(price, 'f', 2) + " " + currency);
  }
  setModified(true);
}

void DeliveryServicePlus::setValue(const QVariant &val) {
  int i = -1;
  if (val.type() == QVariant::Int) {
    i = val.toInt();
  } else {
    i = m_serviceBox->findData(val.toString());
  }

  if (i > 0) {
    p_service = i;
    m_serviceBox->setCurrentIndex(i);
    setModified(true);
  }
}

void DeliveryServicePlus::reset() {
  m_serviceBox->setCurrentIndex(0);
  p_service = 0;
  setModified(false);
}

void DeliveryServicePlus::setFocus() { m_serviceBox->setFocus(); }

void DeliveryServicePlus::loadSqlDataset() {
  QString sql("SELECT d_id, d_name FROM ref_delivery_service");
  sql.append(" ORDER BY d_id ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    if (m_serviceBox->count() >= 1) {
      m_serviceBox->clear();
    }
    while (q.next()) {
      int i = q.value("d_id").toInt();
      QString t = q.value("d_name").toString();
      m_serviceBox->insertItem(i, t);
    }
  }
}

int DeliveryServicePlus::packageId() { return m_packageBox->currentIndex(); }

int DeliveryServicePlus::serviceId() { return m_serviceBox->currentIndex(); }

const QVariant DeliveryServicePlus::value() { return p_service; }

bool DeliveryServicePlus::isValid() {
  if (isRequired() && m_serviceBox->currentIndex() == 0)
    return false;

  return true;
}

void DeliveryServicePlus::setInfo(const QString &info) {
  m_serviceBox->setToolTip(info);
}

const QString DeliveryServicePlus::info() { return m_serviceBox->toolTip(); }

const QString DeliveryServicePlus::notes() {
  return tr("Delevery Service is needet!");
}
