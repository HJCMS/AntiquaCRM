// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstatusbar.h"

#include <QLayout>
#include <QMenu>

BookStatusBar::BookStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("book_status_bar");

  QHBoxLayout *layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  btn_create = new QPushButton(m_frame);
  btn_create->setText(tr("Create"));
  btn_create->setToolTip(tr("Create a new entry."));
  btn_create->setIcon(getIcon("db_add"));
  btn_create->setEnabled(false);
  layout->addWidget(btn_create);

  btn_history = new QPushButton(m_frame);
  btn_history->setText(tr("History"));
  btn_history->setIcon(getIcon("view_list"));
  layout->addWidget(btn_history);
  m_frame->setLayout(layout);

  setHistoryActionMenu(btn_history);
  connect(btn_create, SIGNAL(clicked()), SIGNAL(sendCreateEntry()));
}

void BookStatusBar::setHistoryAction(int index) {
  TabStatusBar::History hist = static_cast<TabStatusBar::History>(index);
  QString q;
  QString year("date_part('year',ib_changed)=date_part('year',CURRENT_DATE)");
  switch (hist) {
  case (History::Today): {
    q.append("DATE(ib_changed)=CURRENT_DATE");
    break;
  }

  case (History::Yesterday): {
    q.append("DATE(ib_changed)=(CURRENT_DATE -1)");
    break;
  }

  case (History::ThisWeek): {
    q.append("date_part('week',ib_changed)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + year);
    break;
  }

  case (History::LastWeek): {
    q.append("date_part('week',ib_changed)=date_part('week',CURRENT_DATE -7)");
    q.append(" AND " + year);
    break;
  }

  case (History::ThisMonth): {
    q.append("date_part('month',ib_changed)=date_part('month',CURRENT_DATE)");
    q.append(" AND " + year);
    break;
  }

  case (History::LastMonth): {
    q.append(
        "date_part('month',ib_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year);
    break;
  }

  case (History::ThisYear): {
    q.append(year + " AND ib_count>0");
    break;
  }

  case (History::NOIMAGE): {
    q.append("DATE(ib_changed)>(CURRENT_DATE - 5) AND im_id IS NULL");
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
