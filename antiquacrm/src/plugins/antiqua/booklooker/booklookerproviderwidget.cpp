// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerproviderwidget.h"
#include "booklookerconfig.h"

#include <QVBoxLayout>

BooklookerProviderWidget::BooklookerProviderWidget(const QString &widgetId,
                                               QWidget *parent)
    : Antiqua::ProviderWidget{widgetId, parent} {
  setObjectName("booklooker_main");
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->addStretch(1);
  setLayout(layout);
}
