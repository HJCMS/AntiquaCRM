// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsmenu.h"
#include "tabsindex.h"

namespace AntiquaCRM {

TabsMenu::TabsMenu(QMenuBar *parent) : QMenu{parent} {
  setObjectName("tabs-view_menu");
  setTitle(tr("Views"));

  m_mapper = new QSignalMapper(this);
  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendOpenTab(const QString &)));
}

void TabsMenu::addAction(AntiquaCRM::TabsIndex *tab) {
  QAction *_ac = QMenu::addAction(tab->windowIcon(), tab->getTitle());
  // WARNING - Id must equal to tab->tabIndexId()!
  _ac->setObjectName(tab->tabIndexId());
  connect(_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  m_mapper->setMapping(_ac, tab->tabIndexId());
}

TabsMenu::~TabsMenu() {}

} // namespace AntiquaCRM
