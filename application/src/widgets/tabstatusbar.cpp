// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatusbar.h"

#include <QDebug>
#include <QHash>
#include <QSignalMapper>

TabStatusBar::TabStatusBar(QWidget *parent) : QStatusBar{parent} {
  setSizeGripEnabled(false);
  setContentsMargins(0, 0, 4, 5);

  m_frame = new QFrame(this);
  m_frame->setContentsMargins(0, 0, 0, 0);
  layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  m_frame->setLayout(layout);
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

void TabStatusBar::addButton(QPushButton *btn) {
  layout->addWidget(btn);
}

QPushButton *TabStatusBar::createButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(QIcon("://icons/db_add.png"));
  btn->setToolTip(tr("Create a new entry."));
  btn->setStatusTip(btn->toolTip());
  if (title.isEmpty())
    btn->setText(tr("Create"));
  else
    btn->setText(title);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendCreateEntry()));
  addButton(btn);
  return btn;
}

QPushButton *TabStatusBar::historyButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(QIcon("://icons/view_list.png"));
  btn->setToolTip(tr("History menue"));
  btn->setStatusTip(btn->toolTip());
  if (title.isEmpty())
    btn->setText(tr("History"));
  else
    btn->setText(title);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendDefaultView()));
  addButton(btn);
  return btn;
}

QPushButton *TabStatusBar::defaultViewButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(QIcon("://icons/spreadsheet.png"));
  btn->setToolTip(tr("Push to load the Standard view."));
  btn->setStatusTip(btn->toolTip());
  if (title.isEmpty())
    btn->setText(tr("Default view"));
  else
    btn->setText(title);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendDefaultView()));
  addButton(btn);
  return btn;
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

void TabStatusBar::setStockEnabled(bool b) { SearchWithStock = b; }
