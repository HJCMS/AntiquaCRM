// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storageedit.h"
#include "sqlcore.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSizePolicy>

StorageEdit::StorageEdit(QWidget *parent) : QFrame{parent} {
  if (objectName().isEmpty())
    setObjectName("StorageEdit");

  setContentsMargins(0, 0, 0, 0);
  setModified(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_storage = new QComboBox(this);
  m_storage->setInsertPolicy(QComboBox::NoInsert);
  m_storage->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  m_storage->setEditable(false);
  layout->addWidget(m_storage);

  m_search = new QLineEdit(this);
  m_search->setMaximumWidth(100);
  m_search->setPlaceholderText(tr("Storage"));
  layout->addWidget(m_search);

  setLayout(layout);

  connect(m_search, SIGNAL(textChanged(const QString &)), this,
          SLOT(filterChanged(const QString &)));
  connect(m_storage, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void StorageEdit::dataChanged(int) { setModified(true); }

void StorageEdit::filterChanged(const QString &filter) {
  int index = 0;
  if (filter.length() < 2) {
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

void StorageEdit::setModified(bool b) { modified = b; }

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

void StorageEdit::loadStorageData() { setStorageData(); }

void StorageEdit::setValue(const QVariant &val) {
  if (m_storage->count() < 2) {
    qWarning("Storage isEmpty. Did you forget loadStorageData() first?");
    return;
  }
  m_storage->setCurrentIndex(val.toInt());
  setModified(false);
}

void StorageEdit::reset() {
  m_storage->setCurrentIndex(0);
  setModified(false);
}

void StorageEdit::setRequired(bool b) { required = b; }

bool StorageEdit::isRequired() { return required; }

bool StorageEdit::hasModified() {
  if (m_storage->currentIndex() == 0)
    return true;

  return modified;
}

const QVariant StorageEdit::value() {
  return QVariant(m_storage->currentIndex());
}

bool StorageEdit::isValid() {
  if (required && (m_storage->currentIndex() == 0))
    return false;

  if (m_storage->currentIndex() > 0)
    return true;

  return false;
}

const QString StorageEdit::notes() {
  return tr("The Storage location is required and must set.");
}
