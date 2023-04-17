// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabwidget.h"
#include "tabsbar.h"
#include "tabsindex.h"

#include <AntiquaInput>

namespace AntiquaCRM {

TabWidget::TabWidget(QWidget *parent) : QTabWidget{parent} {
  setContentsMargins(1, 1, 1, 1);
  setTabsClosable(false);
  m_cfg = new AntiquaCRM::ASettings(this);
  bool _wheel_support =
      m_cfg->groupValue("window_behavior", "mouse_wheel_support", false)
          .toBool();
  m_tabBar = new AntiquaCRM::TabsBar(this, _wheel_support);
  setTabBar(m_tabBar);
}

void TabWidget::tabInserted(int index) {
  AntiquaCRM::TabsIndex *m_tab =
      qobject_cast<AntiquaCRM::TabsIndex *>(tabWithIndex(index));
  if (m_tab != nullptr) {
    m_tabBar->setTabCloseable(index, m_tab->isClosable());
  } else {
    m_tabBar->setTabCloseable(index, false);
  }
}

void TabWidget::setViewTab() {
  QString _id = sender()->objectName();
  if (!_id.isEmpty())
    setCurrentIndex(indexByName(_id));
}

void TabWidget::setCurrentTab(const QString &id) {
  if (!id.isEmpty())
    setCurrentIndex(indexByName(id));
}

int TabWidget::indexByName(const QString &id) {
  if (id.isEmpty())
    return -1;

  for (int i = 0; i < count(); i++) {
    if (widget(i)->objectName() == id) {
      return i;
    }
  }
  return -1;
}

int TabWidget::registerTab(AntiquaCRM::TabsIndex *tab, const QString &title) {
  QString _id = tab->tabIndexId();
  if (_id.isEmpty()) {
    qWarning("Invalid tab „IndexId“ - rejected!");
    return -1;
  }
  int index = addTab(tab, tab->windowIcon(), title);
  if (index >= 0) {
    m_tabBar->setTabCloseable(index, tab->isClosable());
    tab->onEnterChanged();
  }
  return index;
}

const QIcon TabWidget::defaultIcon() {
  return AntiquaApplIcon("action-edit-document");
}

AntiquaCRM::TabsIndex *TabWidget::tabWithIndex(int index) {
  return qobject_cast<AntiquaCRM::TabsIndex *>(widget(index));
}

AntiquaCRM::TabsIndex *TabWidget::tabWithName(const QString &name) {
  return tabWithIndex(indexByName(name));
}

bool TabWidget::beforeCloseAllTabs() {
  qDebug() << Q_FUNC_INFO << "TODO";
  return true;
}

} // namespace AntiquaCRM
