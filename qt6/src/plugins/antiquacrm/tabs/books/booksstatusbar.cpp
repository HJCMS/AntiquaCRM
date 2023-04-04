// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksstatusbar.h"

BooksStatusBar::BooksStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  btn_create = createButton();
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void BooksStatusBar::setHistoryAction(int index) {
  AntiquaCRM::TabsStatusBar::History hist =
      static_cast<AntiquaCRM::TabsStatusBar::History>(index);
  QString q;
  QString year("date_part('year',ib_changed)=date_part('year',CURRENT_DATE)");
  QString stock = (SearchWithStock ? " AND ib_count>0" : "");

  switch (hist) {
  case (AntiquaCRM::TabsStatusBar::History::Today): {
    q.append("DATE(ib_changed)=CURRENT_DATE");
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::Yesterday): {
    q.append("DATE(ib_changed)=(CURRENT_DATE -1)" + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::ThisWeek): {
    q.append("date_part('week',ib_changed)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::LastWeek): {
    q.append("date_part('week',ib_changed)=date_part('week',CURRENT_DATE -7)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::ThisMonth): {
    q.append("date_part('month',ib_changed)=date_part('month',CURRENT_DATE)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::LastMonth): {
    q.append(
        "date_part('month',ib_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year + stock);
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::ThisYear): {
    q.append(year + " AND ib_count>0");
    break;
  }

  case (AntiquaCRM::TabsStatusBar::History::NOIMAGE): {
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

void BooksStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool BooksStatusBar::isCreateButtonEnabled() { return btn_create->isEnabled(); }
