// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsmenu.h"

StatisticsMenu::StatisticsMenu(QMenuBar *parent) : AntiquaCRM::AMenu{parent} {
  setObjectName("antiqua_statistics_menu");
  setTitle(tr("Statistics"));
  setIcon(AntiquaCRM::antiquaIcon("office-chart-area"));
}

void StatisticsMenu::addActions() {
  /*
    connect(Action, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(Action, String);
  */
}
