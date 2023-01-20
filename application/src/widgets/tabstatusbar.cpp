// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatusbar.h"

#include <QDebug>
#include <QSignalMapper>

TabStatusBar::TabStatusBar(QWidget *parent) : QStatusBar{parent} {
  setSizeGripEnabled(false);
  setContentsMargins(0, 0, 4, 5);

  m_frame = new QFrame(this);
  m_frame->setContentsMargins(0, 0, 0, 0);
  insertPermanentWidget(0, m_frame, 0);

  btn_refresh = new QPushButton(this);
  btn_refresh->setIcon(getIcon("action_reload"));
  btn_refresh->setText(tr("Update"));
  btn_refresh->setToolTip(tr("Update current View"));
  btn_refresh->setWhatsThis(tr("When pressed the table query will reloaded."));
  insertPermanentWidget(1, btn_refresh, 0);

  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(sendReloadView()));
}

const QMap<TabStatusBar::History, QString> TabStatusBar::historyItems() {
  QMap<TabStatusBar::History, QString> items;
  items.insert(History::Today, tr("Today"));
  items.insert(History::Yesterday, tr("Yesterday"));
  items.insert(History::LastWeek, tr("Last Week"));
  items.insert(History::LastMonth, tr("Last Month"));
  items.insert(History::ThisWeek, tr("This Week"));
  items.insert(History::ThisMonth, tr("This Month"));
  items.insert(History::ThisYear, tr("This Year"));
  items.insert(History::NOIMAGE, tr("Past Days edited (no Image)"));
  return items;
}

void TabStatusBar::setHistoryActionMenu(QPushButton *parent) {
  // Mapper für Verlaufssignale
  QSignalMapper *m_mapper = new QSignalMapper(parent);
  QMenu *m_menu = new QMenu(parent);
  QIcon icon = getIcon("view_books");
  QStringList entries;
  QMapIterator<TabStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_menu->addAction(icon, it.value());
    connect(ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(ac, it.key());
  }
  parent->setMenu(m_menu);
  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

const QIcon TabStatusBar::getIcon(const QString &name) const {
  return QIcon(":icons/" + name + ".png");
}
