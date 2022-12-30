// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PAYMENTSINYEAR_H
#define ANTIQUACRM_PAYMENTSINYEAR_H

#include <AGlobal>
#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class CountAxis;
class YearAxis;

class PaymentsInYear final : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  YearAxis *m_axisYear;
  CountAxis *m_axisCount;

public:
  explicit PaymentsInYear(const QDate &date, QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PAYMENTSINYEAR_H
