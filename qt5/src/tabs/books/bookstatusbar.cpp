// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstatusbar.h"

#include <QLayout>
#include <QMenu>

BookStatusBar::BookStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("books_status_bar");

  btn_create = createButton();
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void BookStatusBar::setHistoryAction(int index) {
  TabStatusBar::History hist = static_cast<TabStatusBar::History>(index);
  QString q;
  QString year("date_part('year',ib_changed)=date_part('year',CURRENT_DATE)");
  QString stock = (SearchWithStock ? " AND ib_count>0" : "");

  switch (hist) {
  case (History::Today): {
    q.append("DATE(ib_changed)=CURRENT_DATE");
    break;
  }

  case (History::Yesterday): {
    q.append("DATE(ib_changed)=(CURRENT_DATE -1)" + stock);
    break;
  }

  case (History::ThisWeek): {
    q.append("date_part('week',ib_changed)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + year + stock);
    break;
  }

  case (History::LastWeek): {
    q.append("date_part('week',ib_changed)=date_part('week',CURRENT_DATE -7)");
    q.append(" AND " + year + stock);
    break;
  }

  case (History::ThisMonth): {
    q.append("date_part('month',ib_changed)=date_part('month',CURRENT_DATE)");
    q.append(" AND " + year + stock);
    break;
  }

  case (History::LastMonth): {
    q.append(
        "date_part('month',ib_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year + stock);
    break;
  }

  case (History::ThisYear): {
    q.append(year + " AND ib_count>0");
    break;
  }

  case (History::NOIMAGE): {
    q.append("DATE(ib_changed)>(CURRENT_DATE - 5) AND im_id IS NULL");
    q.append(" AND ib_count>0");
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!q.isEmpty())
    emit sendHistoryQuery(q);
}

void BookStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool BookStatusBar::isCreateButtonEnabled() { return btn_create->isEnabled(); }
