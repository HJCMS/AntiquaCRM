// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookbinding.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QJsonObject>

BookBinding::BookBinding(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Book binding"));
  m_layout->addWidget(m_box);
  loadDataset();
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void BookBinding::loadDataset() {
  AntiquaCRM::ASharedDataFiles dataFiles;
  QJsonDocument doc = dataFiles.getJson("bookbindings");
  if (doc.isEmpty()) {
    qWarning("Bookbinding: JsonDocument is Empty!");
    return;
  }

  QJsonArray arr = doc.object().value("bookbindings").toArray();
  for (int i = 0; i < arr.count(); i++) {
    QJsonObject obj = arr[i].toObject();
    QString text = (i == 0) ? "name" : "description";
    m_box->insertItem(i, obj.value(text).toString(), obj.value("id").toInt());
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

const QVariant BookBinding::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toInt();
}

bool BookBinding::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void BookBinding::setInfo(const QString &info) { m_box->setToolTip(info); }

const QString BookBinding::info() { return toolTip(); }

const QString BookBinding::notes() {
  return tr("Missing required Book binding!");
}