// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersstatusbar.h"

#include <QLayout>
#include <QMenu>

CustomersStatusBar::CustomersStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("customer_status_bar");

  QHBoxLayout *layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  btn_create = new QPushButton(m_frame);
  btn_create->setText(tr("Create customer"));
  btn_create->setToolTip(tr("Create a new entry."));
  btn_create->setIcon(getIcon("db_add"));
  btn_create->setEnabled(false);
  btn_create->setVisible(false);
  layout->addWidget(btn_create);

  btn_history = new QPushButton(m_frame);
  btn_history->setText(tr("History"));
  btn_history->setIcon(getIcon("view_list"));
  layout->addWidget(btn_history);
  m_frame->setLayout(layout);

  setHistoryActionMenu(btn_history);
  connect(btn_create, SIGNAL(clicked()), SIGNAL(sendCreateEntry()));
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
    q.append(
        "date_part('month',c_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year);
    break;
  }

  case (History::ThisYear): {
    q.append(year);
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