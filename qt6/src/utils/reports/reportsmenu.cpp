// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportsmenu.h"

ReportsMenu::ReportsMenu(QMenuBar *parent) : AntiquaCRM::AMenu{parent} {
  setObjectName("antiqua_reports_menu");
  setTitle(tr("Reports"));
  setIcon(AntiquaCRM::antiquaIcon("view-list-details"));
}

void ReportsMenu::addActions() {
  /*
    connect(Action, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(Action, String);
  */
}
