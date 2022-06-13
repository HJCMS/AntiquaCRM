// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storageedit.h"
#include "sqlcore.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QSizePolicy>

StorageEdit::StorageEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("StorageEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_storage = new QComboBox(this);
  m_storage->setInsertPolicy(QComboBox::NoInsert);
  m_storage->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  m_storage->setEditable(false);
  layout->addWidget(m_storage);

  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Search"));
  layout->addWidget(m_search);

  setLayout(layout);

  setModified(false);

  connect(m_search, SIGNAL(textChanged(const QString &)), this,
          SLOT(filterChanged(const QString &)));
  connect(m_storage, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void StorageEdit::dataChanged(int index) {
  p_value = index;
  setModified(true);
}

void StorageEdit::filterChanged(const QString &filter) {
  int index = 0;
  if (filter.length() < 3) {
    index = m_storage->findText(filter, Qt::MatchStartsWith);
  } else {
    index = m_storage->findText(filter, Qt::MatchContains);
  }
  if (index != 0)
    m_storage->setCurrentIndex(index);
}

void StorageEdit::focusInEvent(QFocusEvent *ev) {
  if (ev->type() == QEvent::FocusIn)
    m_storage->setFocus();
}

void StorageEdit::setStorageData() {
  HJCMS::SqlCore *db = new HJCMS::SqlCore(this);
  QString sql("SELECT * FROM ref_storage_location ORDER BY sl_id ASC;");
  QSqlQuery q = db->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int index = q.value(0).toInt();
      QString dispRole;
      if (index == 0) {
        dispRole = tr("Storage Location");
      } else {
        dispRole = q.value(1).toString();
        dispRole.append(" (");
        dispRole.append(q.value(2).toString());
        dispRole.append(")");
      }
      m_storage->insertItem(index, dispRole, q.value(0));
      if (q.value(3).isValid())
        m_storage->setItemData(index, q.value(3).toString(), Qt::ToolTipRole);
    }
    m_storage->setMaxCount(m_storage->count());
  } else {
    qWarning("No SQL QueryResult in StorageEdit.");
  }
}

void StorageEdit::loadDataset() { setStorageData(); }

void StorageEdit::setValue(const QVariant &val) {
  if (m_storage->count() < 2) {
    qWarning("Storage isEmpty. Did you forget loadStorageData() first?");
    return;
  }
  int index = -1;
  if (val.type() == QVariant::Int)
    index = m_storage->findData(val, Qt::UserRole);
  else if (val.type() == QVariant::String)
    index = m_storage->findData(val, Qt::DisplayRole);

  if (index < 1)
    return;

  p_value = index;
  m_storage->setCurrentIndex(index);
}

void StorageEdit::reset() {
  m_storage->setCurrentIndex(0);
  m_search->clear();
  setModified(false);
}

void StorageEdit::setFocus() { m_storage->setFocus(); }

const QString StorageEdit::description() {
  int i = m_storage->currentIndex();
  return m_storage->itemData(i, Qt::DisplayRole).toString();
}

const QVariant StorageEdit::value() {
  int i = m_storage->currentIndex();
  return m_storage->itemData(i, Qt::UserRole);
}

bool StorageEdit::isValid() {
  if (isRequired() && (m_storage->currentIndex() == 0))
    return false;

  if (m_storage->currentIndex() > 0)
    return true;

  return false;
}

void StorageEdit::setInfo(const QString &info) { setToolTip(info); }

const QString StorageEdit::info() { return toolTip(); }

const QString StorageEdit::notes() {
  return tr("Storage location is required and must set.");
}
