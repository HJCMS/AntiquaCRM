// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverpackagebox.h"

#include <QDebug>

DeliverPackageBox::DeliverPackageBox(QWidget *parent) : QComboBox{parent} {
  m_sql = new AntiquaCRM::ASqlCore(this);
}

void DeliverPackageBox::setCurrentPackages(int srv) {
  clear();
  QString sql("SELECT d_cid,d_class,d_definition,d_default");
  sql.append(" FROM ref_delivery_cost WHERE d_srv=");
  sql.append(QString::number(srv));
  sql.append(" ORDER BY d_international ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    int i = 0;
    while (q.next()) {
      QString txt = q.value("d_class").toString();
      txt.append(" ");
      txt.append(q.value("d_definition").toString());
      insertItem(i, txt);

      int cid = q.value("d_cid").toInt();
      setItemData(i, cid, Qt::UserRole);

      if (q.value("d_default").toBool())
        setCurrentIndex(i);

      i++;
    }
    emit validServiceChanged((count() > 0));
  }
}

void DeliverPackageBox::setCurrentPackageId(int cid) {
  for (int i = 0; i < count(); i++) {
    if (itemData(i, Qt::UserRole).toInt() == cid) {
      setCurrentIndex(i);
      break;
    }
  }
}

int DeliverPackageBox::getCurrentPackageId() {
  QVariant val = itemData(currentIndex(), Qt::UserRole);
  return (val.isValid()) ? val.toInt() : -1;
}

qreal DeliverPackageBox::getPackagePrice(int cid) {
  QString sql("SELECT d_price FROM ");
  sql.append("ref_delivery_cost WHERE d_cid=");
  sql.append(QString::number(cid) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    return q.value("d_price").toDouble();
  }
  return 0.00;
}

bool DeliverPackageBox::isInternational() {
  QString sql("SELECT d_international FROM ");
  sql.append("ref_delivery_cost WHERE d_cid=");
  sql.append(QString::number(getCurrentPackageId()) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    return q.value("d_international").toBool();
  }
  return false;
}
