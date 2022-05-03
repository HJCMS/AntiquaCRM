// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiqua_global.h"
#include "datadelegation.h"

#include <QDebug>

DataDelegation::DataDelegation(QTableView *parent) : QItemDelegate{parent} {
  setObjectName("DataDelegation");

}

