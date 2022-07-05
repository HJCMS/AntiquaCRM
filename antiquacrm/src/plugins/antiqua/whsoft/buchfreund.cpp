// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"

// #include <QDebug>

Buchfreund::Buchfreund(const QString &widgetId, QWidget *parent)
    : Antiqua::ProviderWidget{widgetId, parent} {
  setObjectName("buchfreund_main");
}
