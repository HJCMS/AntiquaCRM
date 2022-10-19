// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagelocation.h"

#include <AntiquaCRM>

StorageLocation::StorageLocation(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Storage Location"));
  m_box->insertItem(0, tr("Without disclosures"), QString());
  m_box->setMaxVisibleItems(20);
  m_layout->addWidget(m_box);
  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Storage Location"));
  m_search->setMinimumWidth(130);
  m_layout->addWidget(m_search);
  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));

  connect(m_search, SIGNAL(textChanged(const QString &)), this,
          SLOT(filterChanged(const QString &)));
}

void StorageLocation::loadDataset() {
  QString key("storagelocations");
  AntiquaCRM::ASharedDataFiles dataFile;
  if (dataFile.fileExists(key)) {
    QJsonDocument jdoc = dataFile.getJson(key);
    QJsonArray arr = jdoc.object().value(key).toArray();
    if (arr.size() > 0) {
      for (int i = 0; i < arr.size(); i++) {
        QJsonObject jo = arr[i].toObject();
        int index = jo.value("id").toInt();
        if (index == 0)
          continue;

        QString display = jo.value("storage").toString();
        display.append(" - ");
        display.append(jo.value("identifier").toString());
        m_box->insertItem(index, display, index);
      }
    }
  } else {
    qWarning("Storage:No storagelocations.json - fallback to SQL query!");
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QString sql("SELECT * FROM ref_storage_location ORDER BY sl_id;");
    QSqlQuery q = m_sql->query(sql);
    if (q.size() > 0) {
      while (q.next()) {
        int index = q.value("sl_id").toInt();
        if (index == 0)
          continue;

        QString display = q.value("sl_storage").toString();
        display.append(" - ");
        display.append(q.value("sl_identifier").toString());
        m_box->insertItem(index, display, index);
      }
    }
  }
}

void StorageLocation::dataChanged(int index) {
  Q_UNUSED(index);
  setModified(true);
}

void StorageLocation::filterChanged(const QString &filter) {
  int index = 0;
  if (filter.length() < 3) {
    index = m_box->findText(filter, Qt::MatchStartsWith);
  } else {
    index = m_box->findText(filter, Qt::MatchContains);
  }
  if (index != 0)
    m_box->setCurrentIndex(index);
}

void StorageLocation::reset() {
  m_search->clear();
  m_box->setCurrentIndex(0);
  setModified(false);
}

void StorageLocation::setValue(const QVariant &val) {
  int index = -1;
  if (val.type() == QVariant::Int)
    index = m_box->findData(val, Qt::UserRole);
  else if (val.type() == QVariant::String)
    index = m_box->findData(val, Qt::DisplayRole);

  if (index < 1)
    return;

  m_box->setCurrentIndex(index);
}

void StorageLocation::setFocus() { m_box->setFocus(); }

void StorageLocation::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant StorageLocation::value() {
  return m_box->itemData(m_box->currentIndex(), Qt::UserRole);
}

bool StorageLocation::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return (m_box->currentIndex() > 0);
}

void StorageLocation::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString StorageLocation::info() { return m_box->toolTip(); }

const QString StorageLocation::notes() {
  return tr("Storage location is required and must set.");
}
