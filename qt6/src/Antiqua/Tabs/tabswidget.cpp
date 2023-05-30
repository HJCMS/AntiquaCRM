// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabswidget.h"
#include "tabsbar.h"
#include "tabsindex.h"

#include <AntiquaInput>

namespace AntiquaCRM {

TabsWidget::TabsWidget(QWidget *parent) : QTabWidget{parent} {
  setContentsMargins(1, 1, 1, 1);
  setTabsClosable(false);
  m_cfg = new AntiquaCRM::ASettings(this);
  bool _wheel_support =
      m_cfg->groupValue("window_behavior", "mouse_wheel_support", false)
          .toBool();
  m_tabBar = new AntiquaCRM::TabsBar(this, _wheel_support);
  setTabBar(m_tabBar);

  connect(this, SIGNAL(currentChanged(int)), SLOT(setEnterChanged(int)));
}

void TabsWidget::tabInserted(int index) {
  AntiquaCRM::TabsIndex *m_tab = tabWithIndex(index);
  if (m_tab != nullptr) {
    m_tabBar->setTabCloseable(index, m_tab->isClosable());
  } else {
    m_tabBar->setTabCloseable(index, false);
  }
}

void TabsWidget::setEnterChanged(int index) {
  AntiquaCRM::TabsIndex *m_tab = tabWithIndex(index);
  if (m_tab != nullptr) {
    if (m_tab->currentView() == TabsIndex::ViewPage::MainView) {
      m_tab->onEnterChanged();
    }
#ifdef ANTIQUA_DEVELOPEMENT
    else {
      qDebug() << Q_FUNC_INFO << m_tab->getTitle() << "no main view";
    }
#endif
  }
}

void TabsWidget::setCurrentTab(const QString &objname) {
  if (objname.isEmpty())
    return;

  setCurrentIndex(indexByName(objname));
}

void TabsWidget::setViewTab() {
  QString _id = sender()->objectName();
  if (_id.isEmpty())
    return;

  setCurrentIndex(indexByName(_id));
}

int TabsWidget::indexByName(const QString &objname) {
  if (objname.isEmpty())
    return -1;

  for (int i = 0; i < count(); i++) {
    if (widget(i)->objectName() == objname) {
      return i;
    }
  }
  return -1;
}

int TabsWidget::registerTab(AntiquaCRM::TabsIndex *tab, const QString &title) {
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

const QIcon TabsWidget::defaultIcon() {
  return AntiquaApplIcon("action-edit-document");
}

AntiquaCRM::TabsIndex *TabsWidget::tabWithIndex(int index) {
  return qobject_cast<AntiquaCRM::TabsIndex *>(widget(index));
}

AntiquaCRM::TabsIndex *TabsWidget::tabWithName(const QString &name) {
  return tabWithIndex(indexByName(name));
}

bool TabsWidget::unloadTabs() {
  for (int t = 0; t < count(); t++) {
    TabsIndex *m_tab = tabWithIndex(t);
    if (m_tab == nullptr)
      continue;

    if (m_tab->currentView() != TabsIndex::ViewPage::MainView) {
      setCurrentIndex(t);
      emit sendMessage(tr("'%1' Editor is open!").arg(m_tab->getTitle()));
      return false;
    }
    removeTab(t);
  }
  return true;
}

} // namespace AntiquaCRM
