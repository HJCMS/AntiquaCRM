// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "version.h"
#include "datadelegation.h"

#include <QtCore/QDebug>

DataDelegation::DataDelegation(QTableView *parent) : QItemDelegate{parent} {
  setObjectName("DataDelegation");

}

