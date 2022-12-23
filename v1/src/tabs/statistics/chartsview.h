// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CHARTSVIEW_H
#define ANTIQUACRM_CHARTSVIEW_H

#include <QWheelEvent>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

class ChartsView final : public QtCharts::QChartView {
  Q_OBJECT

protected:
  void wheelEvent(QWheelEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;

public:
  explicit ChartsView(QtCharts::QChart *chart, QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_CHARTSVIEW_H
