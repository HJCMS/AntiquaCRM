// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstatusbar.h"

#include <QLayout>
#include <QMenu>

BookStatusBar::BookStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("book_status_bar");

  QHBoxLayout *layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  m_newEntry = new QPushButton(m_frame);
  m_newEntry->setText(tr("Create"));
  m_newEntry->setToolTip(tr("Create a new Book entry."));
  m_newEntry->setIcon(getIcon("db_add"));
  m_newEntry->setEnabled(false);
  layout->addWidget(m_newEntry);

  m_selectHistory = new QPushButton(m_frame);
  m_selectHistory->setText(tr("History"));
  m_selectHistory->setIcon(getIcon("view_list"));
  layout->addWidget(m_selectHistory);
  m_frame->setLayout(layout);

  m_historyMapper = new QSignalMapper(m_selectHistory);
  setHistoryMenu();

  connect(m_newEntry, SIGNAL(clicked()), this, SIGNAL(sendCreateEntry()));
  connect(m_historyMapper, SIGNAL(mappedInt(int)), this,
          SLOT(setHistoryAction(int)));
}

void BookStatusBar::setHistoryMenu() {
  QMenu *m_menu = new QMenu(m_selectHistory);
  QIcon icon = getIcon("view_books");
  QStringList entries;
  QHashIterator<TabStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_menu->addAction(icon, it.value());
    connect(ac, SIGNAL(triggered()), m_historyMapper, SLOT(map()));
    m_historyMapper->setMapping(ac, it.key());
  }
  m_selectHistory->setMenu(m_menu);
}

void BookStatusBar::setCreateButtonEnabled(bool b) {
  m_newEntry->setEnabled(b);
}

bool BookStatusBar::isCreateButtonEnabled() { return m_newEntry->isEnabled(); }

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
    q.append(" AND " + year + " AND ib_count>0");
    break;
  }

  case (History::LastMonth): {
    q.append(
        "date_part('month',ib_changed)=date_part('month',CURRENT_DATE - 31)");
    q.append(" AND " + year + " AND ib_count>0");
    break;
  }

  case (History::ThisYear): {
    q.append(year + " AND ib_count>0");
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!q.isEmpty())
    emit sendHistoryQuery(q);
}

const QHash<TabStatusBar::History, QString> BookStatusBar::historyItems() {
  QHash<TabStatusBar::History, QString> items;
  items.insert(History::Today, tr("Today"));
  items.insert(History::Yesterday, tr("Yesterday"));
  items.insert(History::LastWeek, tr("Last Week"));
  items.insert(History::LastMonth, tr("Last Month"));
  items.insert(History::ThisWeek, tr("This Week"));
  items.insert(History::ThisMonth, tr("This Month"));
  items.insert(History::ThisYear, tr("This Year"));
  return items;
}
