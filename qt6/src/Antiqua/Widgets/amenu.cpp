// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "amenu.h"

namespace AntiquaCRM
{

AMenu::AMenu(QMenuBar* parent) : QMenu{parent} {
  m_mapper = new QSignalMapper(this);
  connect(m_mapper, SIGNAL(mappedString(QString)), SIGNAL(sendOpenTab(QString)));
}

} // namespace AntiquaCRM
