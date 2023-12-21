// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsviewarea.h"

#include <QBrush>
#include <QPainter>
#include <QPalette>
#include <QStringList>

StatisticsViewArea::StatisticsViewArea(QWidget *parent) : QScrollArea{parent} {
  setWidgetResizable(true);
}

const QString StatisticsViewArea::infoText() const {
  QStringList _l;
  // Background info if no chart selected!
  _l << tr("Please select a chart and click display.");
  _l << tr("Not all charts supporting year selection.");
  _l << tr("If the selection for year is not available, this message will be "
           "displayed.");
  return _l.join("\n");
}

void StatisticsViewArea::paintEvent(QPaintEvent *pe) {
  QScrollArea::paintEvent(pe);
  if (widget() == nullptr) {
    QPainter p(viewport());
    p.setBrush(palette().text());
    p.setFont(font());
    p.setOpacity(0.8);
    p.drawText(rect(), Qt::AlignCenter, infoText());
  }
}
