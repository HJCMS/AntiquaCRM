// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksproviderwidget.h"
#include "abebooksconfig.h"

#include <QtWidgets>

AbeBooksProviderWidget::AbeBooksProviderWidget(const QString &widgetId,
                                               QWidget *parent)
    : Antiqua::ProviderWidget{widgetId, parent} {
  setObjectName("buchfreund_main");
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->addStretch(1);
  setLayout(layout);
}
