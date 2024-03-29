// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksstatusbar.h"

BooksStatusBar::BooksStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  // sql feldname für filter setzen
  setFilterName("ib_changed");
  setStockName("ib_count");

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

void BooksStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool BooksStatusBar::isCreateButtonEnabled() { return btn_create->isEnabled(); }
