// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersstatusbar.h"

#include <QLayout>
#include <QMenu>

CustomersStatusBar::CustomersStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("customer_status_bar");

  QHBoxLayout *layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  btn_createEntry = new QPushButton(m_frame);
  btn_createEntry->setText(tr("Create"));
  btn_createEntry->setToolTip(tr("Create a new Customer entry."));
  btn_createEntry->setIcon(getIcon("db_add"));
  btn_createEntry->setEnabled(false);
  layout->addWidget(btn_createEntry);

  btn_history = new QPushButton(m_frame);
  btn_history->setText(tr("History"));
  btn_history->setIcon(getIcon("view_list"));
  layout->addWidget(btn_history);
  m_frame->setLayout(layout);

  m_historyMapper = new QSignalMapper(btn_history);
  setHistoryMenu();

  connect(btn_createEntry, SIGNAL(clicked()), SIGNAL(sendCreateEntry()));
  connect(m_historyMapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

void CustomersStatusBar::setHistoryMenu() {
  QMenu *m_menu = new QMenu(btn_history);
  QIcon icon = getIcon("view_books");
  QStringList entries;
  QHashIterator<TabStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_menu->addAction(icon, it.value());
    connect(ac, SIGNAL(triggered()), m_historyMapper, SLOT(map()));
    m_historyMapper->setMapping(ac, it.key());
  }
  btn_history->setMenu(m_menu);
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
  btn_createEntry->setEnabled(b);
}

bool CustomersStatusBar::isCreateButtonEnabled() {
  return btn_createEntry->isEnabled();
}
