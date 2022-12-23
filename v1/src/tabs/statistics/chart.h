// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CHART_H
#define ANTIQUACRM_CHART_H

#include <QEvent>
#include <QGraphicsItem>
#include <QObject>
#include <QtCharts/QChart>

class Chart final : public QtCharts::QChart {
  Q_OBJECT

protected:
  bool sceneEvent(QEvent *event);

public:
  explicit Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
};

#endif // ANTIQUACRM_CHART_H
