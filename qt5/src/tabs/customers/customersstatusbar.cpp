// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersstatusbar.h"

#include <QLayout>
#include <QMenu>

CustomersStatusBar::CustomersStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("customer_status_bar");
  btn_create = createButton();
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void CustomersStatusBar::setHistoryAction(int index) {
  TabStatusBar::History hist = static_cast<TabStatusBar::History>(index);
  QString q;
  QString year("date_part('year',c_changed)=date_part('year',CURRENT_DATE)");
  switch (hist) {
  case (History::Today): {
    q.append("DATE(c_changed)=CURRENT_DATE");
    break;
  }

  case (History::Yesterday): {
    q.append("DATE(c_changed)=(CURRENT_DATE -1)");
    break;
  }

  case (History::ThisWeek): {
    q.append("date_part('week',c_changed)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + year);
    break;
  }

  case (History::LastWeek): {
    q.append("date_part('week',c_changed)=date_part('week',CURRENT_DATE -7)");
    q.append(" AND " + year);
    break;
  }

  case (History::ThisMonth): {
    q.append("date_part('month',c_changed)=date_part('month',CURRENT_DATE)");
    q.append(" AND " + year);
    break;
  }

  case (History::LastMonth): {
    q.append("date_part('month',c_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND date_part('year',c_since)=date_part('year',CURRENT_DATE)");
    break;
  }

  case (History::ThisYear): {
    q.append("date_part('year',c_since)=date_part('year',CURRENT_DATE)");
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!q.isEmpty())
    emit sendHistoryQuery(q);
}

void CustomersStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
  btn_create->setVisible(b);
}

bool CustomersStatusBar::isCreateButtonEnabled() {
  return btn_create->isEnabled();
}
