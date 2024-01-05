// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdsvinylstatusbar.h"

CDsVinylStatusBar::CDsVinylStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  // sql feldnamen für filter setzen
  setFilterName("cv_changed");
  setStockName("cv_count");

  btn_create = createButton(tr("New entry"),
                            tr("Create a new CD's or Vinyl entry."));
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void CDsVinylStatusBar::setHistoryActionMenu(QPushButton *parent) {
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

void CDsVinylStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool CDsVinylStatusBar::isCreateButtonEnabled() {
  return btn_create->isEnabled();
}
