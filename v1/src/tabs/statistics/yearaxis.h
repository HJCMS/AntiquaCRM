// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_YEARAXIS_H
#define ANTIQUACRM_YEARAXIS_H

#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QtCharts/QDateTimeAxis>

class YearAxis final : public QtCharts::QDateTimeAxis {
  Q_OBJECT

public:
  explicit YearAxis(QObject *parent = nullptr);
  void setStartDate(const QDate &);
  void setEndDate(const QDate &);
};

#endif // ANTIQUACRM_YEARAXIS_H
