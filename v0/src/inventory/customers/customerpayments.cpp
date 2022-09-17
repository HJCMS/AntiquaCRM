// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerpayments.h"

#include <QHeaderView>

CustomerPayments::CustomerPayments(QWidget * parent) : QTableWidget{parent} {
  restore();
}

void CustomerPayments::restore() {
  QStringList headers;
  headers << " " + tr("Order Id") + " ";
  headers << " " + tr("Created") + " ";
  headers << " " + tr("Delivered") + " ";
  setColumnCount(headers.count());
  setRowCount(0);

  setHorizontalHeaderLabels(headers);
  QHeaderView *hView = horizontalHeader();
  hView->setSectionResizeMode(QHeaderView::ResizeToContents);
  hView->setStretchLastSection(true);
}
