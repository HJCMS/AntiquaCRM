// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersstatusbar.h"

CustomersStatusBar::CustomersStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  btn_create = createButton(tr("Create order"),
                            tr("Create a new Order for this Customer."));
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void CustomersStatusBar::setHistoryActionMenu(QPushButton *parent) {
  // Mapper für Verlaufssignale
  m_mapper = new QSignalMapper(parent);
  QMenu *m_menu = new QMenu(parent);
  QMapIterator<TabsStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    if (it.key() == TabsStatusBar::History::NOIMAGE)
      continue;

    QAction *ac = m_menu->addAction(historyIcon(), it.value());
    connect(ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(ac, it.key());
  }
  parent->setMenu(m_menu);
  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

void CustomersStatusBar::setHistoryAction(int index) {
  TabsStatusBar::History _history = static_cast<TabsStatusBar::History>(index);
  QString _sql;
  QString _year("DATE_PART('year',c_changed)=");
  _year.append("DATE_PART('year',CURRENT_DATE)");

  switch (_history) {
  case (TabsStatusBar::History::Today): {
    _sql.append("DATE(c_changed)=CURRENT_DATE");
    break;
  }

  case (TabsStatusBar::History::Yesterday): {
    _sql.append("DATE(c_changed)=(CURRENT_DATE -1)");
    break;
  }

  case (TabsStatusBar::History::ThisWeek): {
    _sql.append("DATE_PART('week',c_changed)=");
    _sql.append("DATE_PART('week',CURRENT_DATE)");
    _sql.append(" AND " + _year);
    break;
  }

  case (TabsStatusBar::History::LastWeek): {
    _sql.append("DATE_PART('week',c_changed)=");
    _sql.append("DATE_PART('week',CURRENT_DATE -7)");
    _sql.append(" AND " + _year);
    break;
  }

  case (TabsStatusBar::History::ThisMonth): {
    _sql.append("DATE_PART('month',c_changed)=");
    _sql.append("DATE_PART('month',CURRENT_DATE)");
    _sql.append(" AND " + _year);
    break;
  }

  case (TabsStatusBar::History::LastMonth): {
    _sql.append("DATE_PART('month',c_changed)=");
    _sql.append("DATE_PART('month',CURRENT_DATE - 31)");
    _sql.append(" AND " + _year);
    break;
  }

  case (TabsStatusBar::History::ThisYear): {
    _sql.append(_year + " AND c_locked=false");
    break;
  }

  default:
    showMessage(tr("Not implemented in this View!"), 5000);
    _sql.clear();
    break;
  };

  if (!_sql.isEmpty())
    emit sendHistoryQuery(_sql);
}

void CustomersStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool CustomersStatusBar::isCreateButtonEnabled() {
  return btn_create->isEnabled();
}
