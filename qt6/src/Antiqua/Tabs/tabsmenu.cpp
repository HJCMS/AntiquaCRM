// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsmenu.h"

#include "ainputwidget.h"

namespace AntiquaCRM {

TabsMenu::TabsMenu(QMenuBar *parent) : QMenu{parent} {
  setTitle(tr("Views"));

  m_mapper = new QSignalMapper(this);
  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendOpenTab(const QString &)));
}

void TabsMenu::addAction(const QJsonObject &jo) {
  const QString _id = jo.value("id").toString();
  const QString _title = jo.value("title").toString();
  const QIcon _icon = AntiquaCRM::AntiquaApplIcon(jo.value("icon").toString());
  QAction *_ac = QMenu::addAction(_icon, _title);
  _ac->setObjectName(_id);
  connect(_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  m_mapper->setMapping(_ac, _id); // register id
}

bool TabsMenu::exists(const QString &id) {
  if (id.length() < 4) // must contain _tab
    return false;

  return (m_mapper->mapping(id) != nullptr);
}

TabsMenu::~TabsMenu() {}

} // namespace AntiquaCRM
