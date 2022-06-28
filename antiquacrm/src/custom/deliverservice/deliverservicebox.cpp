// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservicebox.h"

#include <SqlCore>

#include <QDebug>

DeliverServiceBox::DeliverServiceBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("deliver_service_box");
}

void DeliverServiceBox::setDeliverServices() {
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  QString sql("SELECT d_id, d_name FROM ref_delivery_service");
  sql.append(" ORDER BY d_id ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    if (count() >= 1) {
      clear();
    }
    while (q.next()) {
      int i = q.value("d_id").toInt();
      QString t = q.value("d_name").toString();
      // qDebug() << Q_FUNC_INFO << i << t;
      insertItem(i, t);
    }
  }
}
