// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstatusbar.h"

#include <QLayout>
#include <QMenu>

BookStatusBar::BookStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("book_status_bar");

  QHBoxLayout *layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  btn_createEntry = new QPushButton(m_frame);
  btn_createEntry->setText(tr("Create"));
  btn_createEntry->setToolTip(tr("Create a new Book entry."));
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

void BookStatusBar::setHistoryMenu() {
  QMenu *m_menu = new QMenu(btn_history);
  QIcon icon = getIcon("view_books");
  QStringList entries;
  QMapIterator<TabStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_menu->addAction(icon, it.value());
    connect(ac, SIGNAL(triggered()), m_historyMapper, SLOT(map()));
    m_historyMapper->setMapping(ac, it.key());
  }
  btn_history->setMenu(m_menu);
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
  btn_createEntry->setEnabled(b);
}

bool BookStatusBar::isCreateButtonEnabled() { return btn_createEntry->isEnabled(); }
