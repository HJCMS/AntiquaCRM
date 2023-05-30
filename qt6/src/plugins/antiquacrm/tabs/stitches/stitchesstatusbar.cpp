// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitchesstatusbar.h"

StitchesStatusBar::StitchesStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  btn_create = createButton();
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void StitchesStatusBar::setHistoryAction(int index) {
  TabsStatusBar::History _history = static_cast<TabsStatusBar::History>(index);
  QString _sql;
  QString _stock = (SearchWithStock ? " AND ip_count>0" : "");
  QString _year("DATE_PART('year',ip_changed)=");
  _year.append("DATE_PART('year',CURRENT_DATE)");

  switch (_history) {
  case (TabsStatusBar::History::Today): {
    _sql.append("DATE(ip_changed)=CURRENT_DATE");
    break;
  }

  case (TabsStatusBar::History::Yesterday): {
    _sql.append("DATE(ip_changed)=(CURRENT_DATE -1)" + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisWeek): {
    _sql.append("DATE_PART('week',ip_changed)=");
    _sql.append("DATE_PART('week',CURRENT_DATE)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::LastWeek): {
    _sql.append("DATE_PART('week',ip_changed)=");
    _sql.append("DATE_PART('week',CURRENT_DATE -7)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisMonth): {
    _sql.append("DATE_PART('month',ip_changed)=");
    _sql.append("DATE_PART('month',CURRENT_DATE)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::LastMonth): {
    _sql.append("DATE_PART('month',ip_changed)=");
    _sql.append("DATE_PART('month',CURRENT_DATE - 31)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisYear): {
    _sql.append(_year + " AND ip_count>0");
    break;
  }

  case (TabsStatusBar::History::NOIMAGE): {
    _sql.append("DATE(ip_changed)>(CURRENT_DATE - 14)");
    _sql.append(" AND im_id IS NULL" + _stock);
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!_sql.isEmpty())
    emit sendHistoryQuery(_sql);
}

void StitchesStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool StitchesStatusBar::isCreateButtonEnabled() {
  return btn_create->isEnabled();
}