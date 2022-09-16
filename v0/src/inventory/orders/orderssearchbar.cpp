// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderssearchbar.h"

#include <QLayout>
#include <QSizePolicy>
#include <QToolButton>

OrdersSearchBar::OrdersSearchBar(QWidget *parent) : QFrame{parent} {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}
