// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvsearchline.h"

CDVSearchLine::CDVSearchLine(QWidget *parent) : SearchLineEdit{parent} {}

const QString CDVSearchLine::getSearch() { return text().trimmed(); }
