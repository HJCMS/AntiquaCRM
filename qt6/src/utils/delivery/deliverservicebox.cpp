// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservicebox.h"

#include <AntiquaCRM>

DeliverServiceBox::DeliverServiceBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("deliver_service_box");
}

void DeliverServiceBox::setCurrentServiceId(int did) {
  for (int i = 0; i < count(); i++) {
    if (itemData(i, Qt::UserRole).toInt() == did) {
      setCurrentIndex(i);
      break;
    }
  }
}

void DeliverServiceBox::initDeliverServices() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString sql("SELECT d_id, d_name FROM ref_delivery_service");
  sql.append(" ORDER BY d_id ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    if (count() >= 1) {
      clear();
    }
    int i = 0;
    while (q.next()) {
      insertItem(i, q.value("d_name").toString());
      setItemData(i, q.value("d_id").toInt(), Qt::UserRole);
      i++;
    }
  }
}

int DeliverServiceBox::getCurrentServiceId() {
  return itemData(currentIndex(), Qt::UserRole).toInt();
}
