// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variousstatusbar.h"

VariousStatusBar::VariousStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  // sql feldname für filter setzen
  setFilterName("va_changed");
  setStockName("va_count");

  btn_create = createButton(tr("New entry"), tr("Create a new entry."));
  setCreateButtonEnabled(false);

  defaultViewButton();

  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

void VariousStatusBar::setHistoryActionMenu(QPushButton *parent) {
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

void VariousStatusBar::setCreateButtonEnabled(bool b) {
  btn_create->setEnabled(b);
}

bool VariousStatusBar::isCreateButtonEnabled() {
  return btn_create->isEnabled();
}
