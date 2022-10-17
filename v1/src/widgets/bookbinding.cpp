// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookbinding.h"

#include <AntiquaCRM>
#include <QJsonObject>

BookBinding::BookBinding(QWidget *parent) : InputEdit{parent} {

  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Book binding"));
  m_box->addItem(tr("Without disclosures"), QString());
  m_layout->addWidget(m_box);

  loadDataset();
  setRequired(true);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void BookBinding::loadDataset() {
  AntiquaCRM::ASharedDataFiles dataFiles;
  QJsonDocument doc = dataFiles.getJson("bookbindings");
  if (doc.isEmpty()) {
    qWarning("Bookbinding: bookbindings.json not found!");
    return;
  }

  QJsonArray arr = doc.object().value("bookbindings").toArray();
  for (int i = 0; i < arr.count(); i++) {
    QJsonObject obj = arr[i].toObject();
    m_box->addItem(obj.value("description").toString(),
                   obj.value("id").toInt());
  }
}

void BookBinding::dataChanged(int) { setModified(true); }

void BookBinding::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void BookBinding::setValue(const QVariant &val) {
  int id = val.toInt();
  if (id > 0) {
    int index = m_box->findData(id, Qt::UserRole, Qt::MatchExactly);
    m_box->setCurrentIndex(index);
  }
}

void BookBinding::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void BookBinding::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant BookBinding::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toInt();
}

bool BookBinding::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void BookBinding::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString BookBinding::info() { return toolTip(); }

const QString BookBinding::notes() {
  return tr("Missing required Book binding!");
}
