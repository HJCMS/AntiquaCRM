// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverpackagebox.h"

#include <QDebug>

DeliverPackageBox::DeliverPackageBox(QWidget *parent) : QComboBox{parent} {
  m_sql = new HJCMS::SqlCore(this);
}

void DeliverPackageBox::setCurrentService(int id) {
  clear();
  QString sql("SELECT d_cid,d_class,d_definition,d_default");
  sql.append(" FROM ref_delivery_cost WHERE d_srv=");
  sql.append(QString::number(id));
  sql.append(" ORDER BY d_international ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int i = q.value("d_cid").toInt();
      QString t = q.value("d_class").toString();
      t.append(" ");
      t.append(q.value("d_definition").toString());
      insertItem(i, t);
      if (q.value("d_default").toBool())
        setCurrentIndex(i);
    }
    emit validServiceChanged((count() > 0));
  }
}

qreal DeliverPackageBox::getPackagePrice(int index) {
  QString sql("SELECT DISTINCT d_price ");
  sql.append("FROM ref_delivery_cost WHERE d_cid=");
  sql.append(QString::number(index));
  sql.append(" LIMIt 1;");
  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    return q.value("d_price").toDouble();
  }
  return 0.00;
}
