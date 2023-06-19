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

  connect(m_tabBar, SIGNAL(currentChanged(int)), SLOT(setTabChanged(int)));
  connect(m_tabBar, SIGNAL(tabCloseRequested(int)), SLOT(setTabClosed(int)));
}

bool TabsWidget::removeIndex(int index) {
  AntiquaCRM::TabsIndex *m_tab = tabWithIndex(index);
  if (m_tab != nullptr && m_tab->isClosable()) {
    if (!m_tab->isWindowModified()) {
      removeTab(index);
      return true;
    }
    QString title = m_tab->windowTitle();
    emit sendMessage(tr("Unsaved changes for tab '%1'!").arg(title));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "Can't close this tab, unsaved changes!" << title;
#endif
  }
  return false;
}

void TabsWidget::tabInserted(int index) {
  AntiquaCRM::TabsIndex *m_tab = tabWithIndex(index);
  if (m_tab != nullptr) {
    m_tabBar->setTabCloseable(index, m_tab->isClosable());
  } else {
    m_tabBar->setTabCloseable(index, false);
  }
}

void TabsWidget::setTabChanged(int index) {
  AntiquaCRM::TabsIndex *m_tab = tabWithIndex(index);
  if (m_tab != nullptr && m_tab->currentView() == TabsIndex::ViewPage::MainView)
    m_tab->onEnterChanged();
}

void TabsWidget::setTabClosed(int index) { removeIndex(index); }

void TabsWidget::setCurrentTab(const QString &name) {
  QString _id = name.trimmed();
  if (_id.isEmpty() && sender() != nullptr) {
    _id = sender()->objectName();
  }

  if (_id.isEmpty()) {
    qWarning("Missing tab name, setCurrentTab aborted!");
    return;
  }

  setCurrentIndex(indexByName(_id));
}

int TabsWidget::indexByName(const QString &name) {
  if (name.isEmpty())
    return -1;

  for (int i = 0; i < m_tabBar->count(); i++) {
    AntiquaCRM::TabsIndex *m_t = tabWithIndex(i);
    if (m_t == nullptr)
      continue;

    if (m_t->tabIndexId() == name) {
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

  int _index = addTab(tab, tab->windowIcon(), title);
  if (_index >= 0) {
    m_tabBar->setTabCloseable(_index, tab->isClosable());
    tab->onEnterChanged();
  }
  return _index;
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

    if (!removeIndex(t))
      qWarning("Tab %s not removed!", qPrintable(m_tab->tabIndexId()));
  }
  return true;
}

} // namespace AntiquaCRM
