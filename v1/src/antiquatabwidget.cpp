// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "tabbooks.h"

AntiquaTabWidget::AntiquaTabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("tab_widgets_main");

  m_books = new TabBooks(this);
  insertTab(0, m_books, tr("Books"));
}
