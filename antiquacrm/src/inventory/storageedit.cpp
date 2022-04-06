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
  setStorageData();
}

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
  }
}

void StorageEdit::setValue(const QVariant &val) {
  setCurrentIndex(val.toInt());
}

const QVariant StorageEdit::value() { return QVariant(currentIndex()); }
