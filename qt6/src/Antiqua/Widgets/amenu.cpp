// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "amenu.h"

namespace AntiquaCRM {

AMenu::AMenu(QMenuBar *parent) : QMenu{parent} {
  m_mapper = new QSignalMapper(this);
  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendOpenTab(const QString &)));
}

} // namespace AntiquaCRM
