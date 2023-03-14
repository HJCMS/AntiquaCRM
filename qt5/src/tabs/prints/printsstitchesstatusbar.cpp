// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsstitchesstatusbar.h"

#include <QLayout>
#include <QMenu>

PrintsStitchesStatusBar::PrintsStitchesStatusBar(QWidget *parent)
    : TabStatusBar{parent} {
  setObjectName("printsstitches_status_bar");
  btn_create = createButton();
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void PrintsStitchesStatusBar::setHistoryAction(int index) {
  TabStatusBar::History hist = static_cast<TabStatusBar::History>(index);
  QString q;
  QString year("date_part('year',ip_changed)=date_part('year',CURRENT_DATE)");
  switch (hist) {
  case (History::Today): {
    q.append("DATE(ip_changed)=CURRENT_DATE");
    break;
  }

  case (History::Yesterday): {
    q.append("DATE(ip_changed)=(CURRENT_DATE -1)");
    break;
  }

  case (History::ThisWeek): {
    q.append("date_part('week',ip_changed)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + year);
    break;
  }

  case (History::LastWeek): {
    q.append("date_part('week',ip_changed)=date_part('week',CURRENT_DATE -7)");
    q.append(" AND " + year);
    break;
  }

  case (History::ThisMonth): {
    q.append("date_part('month',ip_changed)=date_part('month',CURRENT_DATE)");
    q.append(" AND " + year);
    break;
  }

  case (History::LastMonth): {
    q.append(
        "date_part('month',ip_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year);
    break;
  }

  case (History::ThisYear): {
    q.append(year + " AND ip_count>0");
    break;
  }

  case (History::NOIMAGE): {
    q.append("DATE(ip_changed)>(CURRENT_DATE - 5) AND im_id IS NULL");
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!q.isEmpty())
    emit sendHistoryQuery(q);
}

void PrintsStitchesStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool PrintsStitchesStatusBar::isCreateButtonEnabled() {
  return btn_create->isEnabled();
}
