// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "trust.h"

Trust::Trust() {
  p_index = -1;
  p_title = QString();
}

Trust::Trust(int index, const QString &title) {
  p_index = index;
  p_title = title;
}

int Trust::index() { return p_index; }

const QString Trust::title() { return p_title; }
