// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservice.h"

#include <QtCore>
#include <QtWidgets>

DeliverService::DeliverService(QWidget *parent) : QWidget{parent} {
  setObjectName("deliver_service_view");

  /**
    1 "d_changed"
    2 "d_class"
    3 "d_definition"
    4 "d_description"
    5 "d_id"
    6 "d_infopage"
    7 "d_international"
    8 "d_name"
    9 "d_price"
    10 "d_since"
    11 "d_srv"
    12 "d_website"
  */
  // m_edit = new QWidget(this);

}

void DeliverService::addDeliverServiceEntry(
    const QMap<QString, QVariant> entry) {
  int c = 0;
  QMapIterator<QString, QVariant> it(entry);
  while(it.hasNext()) {
    it.next();
    qDebug() << ++c << it.key();
  }
  qDebug() << Qt::endl;
}
