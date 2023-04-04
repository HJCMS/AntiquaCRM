// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsstatusbar.h"

#include <QDebug>
#include <QHash>
#include <QSignalMapper>

namespace AntiquaCRM {

TabsStatusBar::TabsStatusBar(QWidget *parent) : QStatusBar{parent} {
  setSizeGripEnabled(false);
  setContentsMargins(0, 0, 4, 5);

  m_frame = new QFrame(this);
  m_frame->setContentsMargins(0, 0, 0, 0);
  layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  m_frame->setLayout(layout);
  insertPermanentWidget(0, m_frame, 0);

  btn_refresh = new QPushButton(this);
  btn_refresh->setIcon(getIcon("action-reload"));
  btn_refresh->setText(tr("Update"));
  btn_refresh->setToolTip(tr("Update current View"));
  btn_refresh->setWhatsThis(tr("When pressed the table query will reloaded."));
  insertPermanentWidget(1, btn_refresh, 0);

  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(sendReloadView()));
}

const QMap<TabsStatusBar::History, QString> TabsStatusBar::historyItems() {
  QMap<TabsStatusBar::History, QString> items;
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

void TabsStatusBar::addButton(QPushButton *btn) { layout->addWidget(btn); }

QPushButton *TabsStatusBar::createButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(getIcon("database-add"));
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

QPushButton *TabsStatusBar::historyButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(getIcon("view-list"));
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

QPushButton *TabsStatusBar::defaultViewButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(getIcon("spreadsheet"));
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

void TabsStatusBar::setHistoryActionMenu(QPushButton *parent) {
  // Mapper für Verlaufssignale
  QSignalMapper *m_mapper = new QSignalMapper(parent);
  QMenu *m_menu = new QMenu(parent);
  QIcon icon = getIcon("view-books");
  QStringList entries;
  QMapIterator<TabsStatusBar::History, QString> it(historyItems());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_menu->addAction(icon, it.value());
    connect(ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(ac, it.key());
  }
  parent->setMenu(m_menu);
  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

const QIcon TabsStatusBar::getIcon(const QString &name) {
  QIcon _back("://icons/" + name + ".png");
  return QIcon::fromTheme(name, _back);
}

void TabsStatusBar::setStockEnabled(bool b) { SearchWithStock = b; }

} // namespace AntiquaCRM
