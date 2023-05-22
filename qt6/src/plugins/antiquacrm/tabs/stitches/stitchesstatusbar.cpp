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
  AntiquaCRM::TabsStatusBar::History hist =
      static_cast<AntiquaCRM::TabsStatusBar::History>(index);
  QString q;
  QString year("date_part('year',ip_changed)=date_part('year',CURRENT_DATE)");
  QString stock = (SearchWithStock ? " AND ip_count>0" : "");

  switch (hist) {
  case (AntiquaCRM::TabsStatusBar::History::Today): {
    q.append("DATE(ip_changed)=CURRENT_DATE");
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::Yesterday): {
    q.append("DATE(ip_changed)=(CURRENT_DATE -1)" + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::ThisWeek): {
    q.append("date_part('week',ip_changed)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::LastWeek): {
    q.append("date_part('week',ip_changed)=date_part('week',CURRENT_DATE -7)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::ThisMonth): {
    q.append("date_part('month',ip_changed)=date_part('month',CURRENT_DATE)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::LastMonth): {
    q.append(
        "date_part('month',ip_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::ThisYear): {
    q.append(year + " AND ip_count>0");
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::NOIMAGE): {
    q.append("DATE(ip_changed)>(CURRENT_DATE - 5) AND im_id IS NULL");
    q.append(" AND ip_count>0");
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!q.isEmpty())
    emit sendHistoryQuery(q);
}

void StitchesStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool StitchesStatusBar::isCreateButtonEnabled() { return btn_create->isEnabled(); }
