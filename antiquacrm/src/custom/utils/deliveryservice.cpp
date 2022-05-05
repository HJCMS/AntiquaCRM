#include "deliveryservice.h"

#include <QDebug>
#include <QHBoxLayout>

#include <SqlCore>

DeliveryService::DeliveryService(QWidget *parent) : UtilsMain{parent} {
  setObjectName("delivery_service_edit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  layout->addWidget(m_info);

  m_box = new QComboBox(this);
  m_box->insertItem(0, tr("Without disclosures"));
  layout->addWidget(m_box);

  setLayout(layout);

  setRequired(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(indexChanged(int)));
}

void DeliveryService::indexChanged(int i) {
  p_value = i;
  setModified(true);
}

void DeliveryService::setValue(const QVariant &val) {
  p_value = val.toInt();
  m_box->setCurrentIndex(p_value);
}

void DeliveryService::reset() {
  m_box->setCurrentIndex(0);
  p_value = 0;
  setModified(false);
}

void DeliveryService::setFocus() { m_box->setFocus(); }

void DeliveryService::loadSqlDataset() {
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  QString sql("SELECT d_id,d_name FROM " + sqlTable + " ORDER BY d_id ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    if (m_box->count() >= 1) {
      m_box->clear();
      m_box->insertItem(0, tr("Without disclosures"));
    }
    while (q.next()) {
      int i = q.value("d_id").toInt();
      QString t = q.value("d_name").toString();
      m_box->insertItem(i, t);
    }
  }
}

const QVariant DeliveryService::value() { return p_value; }

bool DeliveryService::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void DeliveryService::setInfo(const QString &info) {
  m_box->setToolTip(info);
  m_info->setText(info + ":");
}

const QString DeliveryService::info() { return m_box->toolTip(); }

const QString DeliveryService::notes() {
  return tr("Delevery Service is needet!");
}
