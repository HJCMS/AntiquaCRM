// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsmenu.h"
#include "antiquaicon.h"

namespace AntiquaCRM {

TabsMenu::TabsMenu(QMenuBar *parent) : QMenu{parent} {
  setTitle(tr("Tabs"));

  m_mapper = new QSignalMapper(this);
  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendOpenTab(const QString &)));
}

void TabsMenu::addAction(const QJsonObject &jo) {
  const QString _id = jo.value("id").toString();
  const QString _title = jo.value("title").toString();
  const QString _tip = jo.value("tip").toString();
  const QIcon _icon = AntiquaCRM::antiquaIcon(jo.value("icon").toString());
  QAction *_ac = QMenu::addAction(_title);
  _ac->setObjectName(_id);
  _ac->setToolTip(_tip);
  _ac->setStatusTip(_tip);
  _ac->setIcon(_icon);

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
