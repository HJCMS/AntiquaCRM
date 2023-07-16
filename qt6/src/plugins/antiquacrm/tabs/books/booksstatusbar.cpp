// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksstatusbar.h"

BooksStatusBar::BooksStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  btn_create = createButton(tr("New entry"), tr("Create a new Book entry."));
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void BooksStatusBar::setHistoryActionMenu(QPushButton *parent) {
  // Mapper für Verlaufssignale
  m_mapper = new QSignalMapper(parent);
  QMenu *m_menu = new QMenu(parent);
  QMapIterator<TabsStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_menu->addAction(historyIcon(), it.value());
    connect(ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(ac, it.key());
  }
  parent->setMenu(m_menu);
  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

void BooksStatusBar::setHistoryAction(int index) {
  TabsStatusBar::History _history = static_cast<TabsStatusBar::History>(index);
  QString _sql;
  QString _stock = (SearchWithStock ? " AND ib_count>0" : "");
  QString _year("DATE_PART('year',ib_changed)=");
  _year.append("DATE_PART('year',CURRENT_DATE)");

  switch (_history) {
  case (TabsStatusBar::History::Today): {
    _sql.append("DATE(ib_changed)=CURRENT_DATE");
    break;
  }

  case (TabsStatusBar::History::Yesterday): {
    _sql.append("DATE(ib_changed)=(CURRENT_DATE -1)" + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisWeek): {
    _sql.append("DATE_PART('week',ib_changed)=");
    _sql.append("DATE_PART('week',CURRENT_DATE)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::LastWeek): {
    _sql.append("DATE_PART('week',ib_changed)=");
    _sql.append("DATE_PART('week',CURRENT_DATE -7)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisMonth): {
    _sql.append("DATE_PART('month',ib_changed)=");
    _sql.append("DATE_PART('month',CURRENT_DATE)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::LastMonth): {
    _sql.append("DATE_PART('month',ib_changed)=");
    _sql.append("DATE_PART('month',CURRENT_DATE - 31)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisYear): {
    _sql.append(_year + " AND ib_count>0");
    break;
  }

  case (TabsStatusBar::History::NOIMAGE): {
    _sql.append("DATE(ib_changed)>(CURRENT_DATE - 14)");
    _sql.append(" AND im_id IS NULL AND ib_count>0");
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!_sql.isEmpty())
    emit sendHistoryQuery(_sql);
}

void BooksStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool BooksStatusBar::isCreateButtonEnabled() { return btn_create->isEnabled(); }
