// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storageedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

StorageEdit::StorageEdit(QWidget *parent) : QComboBox{parent} {
  setObjectName("StorageEdit");
  setMaxVisibleItems(10);
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  setModified(false);

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(dataChanged(int)));
}

void StorageEdit::dataChanged(int) { setModified(true); }

void StorageEdit::setModified(bool b) { modified = b; }

void StorageEdit::setStorageData() {
  QSqlDatabase db(QSqlDatabase::database(sqlConnectionName));
  if (db.isValid()) {
    QString select("SELECT * FROM ");
    select.append("ref_storage_location");
    select.append(" ORDER BY sl_id ASC;");
    QSqlQuery q = db.exec(select);
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
      insertItem(index, dispRole, q.value(0));
      if (q.value(3).isValid())
        setItemData(index, q.value(3).toString(), Qt::ToolTipRole);
    }
  } else {
    qWarning("Can not open Database to read Storage locations.");
  }
}

void StorageEdit::loadStorageData() { setStorageData(); }

void StorageEdit::setValue(const QVariant &val) {
  if (count() < 2) {
    qWarning("StorageEdit isEmpty !"
             "Did you forget loadStorageData() first?");
    return;
  }
  setCurrentIndex(val.toInt());
  setModified(false);
}

void StorageEdit::reset() {
  setCurrentIndex(0);
  setModified(false);
}

void StorageEdit::setRequired(bool b) { required = b; }

bool StorageEdit::isRequired() { return required; }

bool StorageEdit::hasModified() {
  if (currentIndex() == 0)
    return true;

  return modified;
}

const QVariant StorageEdit::value() { return QVariant(currentIndex()); }

bool StorageEdit::isValid() {
  if (required && (currentIndex() == 0))
    return false;

  if (currentIndex() < 1)
    return false;

  return true;
}

const QString StorageEdit::notes() {
  return tr("The Storage location is required and must set.");
}
