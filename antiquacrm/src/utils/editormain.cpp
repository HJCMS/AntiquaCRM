// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editormain.h"

EditorMain::EditorMain(QWidget *parent) : QWidget{parent} {
  setMinimumSize(800, 600);
  setContentsMargins(5, 5, 5, 5);
  m_sql = new HJCMS::SqlCore(this);
}

EditorMain::~EditorMain() { sqlQueryResult.clear(); }
