// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "monthbarset.h"

#include <AntiquaCRM>
#include <QToolTip>

MonthBarSet::MonthBarSet(int year, QChart *parent, MonthBarSet::Type type)
    : QBarSet{QString(), parent}, p_year{year} {
  if (type == MonthBarSet::Type::Sales) {
    setLabel(tr("Volume (%1)").arg(year));
  } else {
    setLabel(tr("Count (%1)").arg(year));
  }
  connect(this, SIGNAL(hovered(bool,int)), SLOT(showToolTip(bool,int)));
}

void MonthBarSet::showToolTip(bool b, int i) {
  if (b && p_sales.size() > i) {
    double _cost = p_sales[(i + 1)];
    if (_cost > 0.00) {
      const QString _money = AntiquaCRM::ATaxCalculator::money(_cost);
      const QString _info = tr("Summary %1 (%2)").arg(_money).arg(p_year);
      QToolTip::showText(QCursor::pos(), _info, nullptr);
      return;
    }
  }
  QToolTip::hideText();
}

void MonthBarSet::setSales(const QMap<int, double> &sales) { p_sales = sales; }

int MonthBarSet::year() const { return p_year; }
