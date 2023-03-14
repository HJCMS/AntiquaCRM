// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagecategory.h"

#include <QLayout>

StorageCategory::StorageCategory(QWidget *parent) : QFrame{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_box = new QComboBox(this);
  layout->addWidget(m_box);

  m_find = new QLineEdit(this);
  m_find->setMinimumWidth(50);
  m_find->setPlaceholderText(tr("Search Category"));
  m_find->setClearButtonEnabled(true);
  layout->addWidget(m_find);

  setLayout(layout);

  restore();

  connect(m_find, SIGNAL(textChanged(const QString &)), this,
          SLOT(searchCategory(const QString &)));

  connect(m_find, SIGNAL(returnPressed()), this, SLOT(onKeyEnterPressed()));
}

void StorageCategory::onKeyEnterPressed() {
  // Ignore Key Press Enter
}

void StorageCategory::searchCategory(const QString &search) {
  if (search.length() < 3)
    return;

  for (int i = 0; i < m_box->count(); i++) {
    QString name = m_box->itemData(i, Qt::DisplayRole).toString();
    if (name.contains(search, Qt::CaseInsensitive)) {
      m_box->setCurrentIndex(i);
      return;
    }
  }
}

void StorageCategory::restore() {
  m_box->clear();
  m_find->clear();
  m_box->addItem(tr("Without disclosures"), "0");
}

void StorageCategory::clear() {
  m_find->clear();
  m_box->setCurrentIndex(0);
}

void StorageCategory::addItems(const QMap<QString, QString> &map) {
  QStringList list;
  QMapIterator<QString, QString> it(map);
  while (it.hasNext()) {
    it.next();
    m_box->addItem(it.key(), it.value());
    list << it.key();
  }

  if (list.count() < 1)
    return;

  m_completer = new QCompleter(list, m_find);
  m_completer->setFilterMode(Qt::MatchContains);
  m_find->setCompleter(m_completer);
}

void StorageCategory::setIndex(int index) { m_box->setCurrentIndex(index); }

int StorageCategory::getIndex() { return m_box->currentIndex(); }

const QString StorageCategory::getTitle() {
  if (m_box->currentIndex() > 0)
    return m_box->currentText().trimmed();

  return QString();
}

void StorageCategory::setValue(qint64 cid) {
  if (cid == 0) {
    m_box->setCurrentIndex(0);
    return;
  }

  QString searchId = QString::number(cid);
  for (int i = 0; i < m_box->count(); i++) {
    QString id = m_box->itemData(i, Qt::UserRole).toString();
    if (id == searchId) {
      m_box->setCurrentIndex(i);
      return;
    }
  }
}

const QString StorageCategory::getValue(qint64 cid) {
  int index = (cid > 0) ? cid : m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toString();
}

int StorageCategory::count() { return m_box->count(); }
