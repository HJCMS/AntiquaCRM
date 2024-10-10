// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabswidget.h"
#include "antiquaicon.h"
#include "tabsbar.h"
#include "tabsindex.h"

namespace AntiquaCRM
{

TabsWidget::TabsWidget(QWidget* parent) : QTabWidget{parent} {
  setContentsMargins(1, 1, 1, 1);
  setTabsClosable(false);

  QStringList _list({"sellers_tab", "orders_tab", "customers_tab"});
  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->beginGroup("window_behavior");
  // Sorting Tabs list
  p_sorting_tabs = m_cfg->value("sorting_tabs", _list).toStringList();
  // Mouse wheel suppport
  p_wheel_support = m_cfg->value("mouse_wheel_support", false).toBool();
  m_cfg->endGroup();

  // tabbar
  m_tabBar = new AntiquaCRM::TabsBar(this, p_wheel_support);
  setTabBar(m_tabBar);

  connect(m_tabBar, SIGNAL(currentChanged(int)), SLOT(setTabChanged(int)));
  connect(m_tabBar, SIGNAL(tabCloseRequested(int)), SLOT(setTabClosed(int)));
}

void TabsWidget::tabInserted(int index) {
  AntiquaCRM::TabsIndex* m_ti = tabIndex(index);
  if (m_ti != nullptr) {
    m_tabBar->setTabCloseable(index, m_ti->isClosable());
  } else {
    m_tabBar->setTabCloseable(index, false);
  }
}

void TabsWidget::tabRemoved(int) {
  if (m_tmp == nullptr)
    return;

  m_tmp->deleteLater();
}

bool TabsWidget::removeIndex(int index) {
  if (!(index > 0))
    return true;

  AntiquaCRM::TabsIndex* m_ti = tabIndex(index);
  if (m_ti != nullptr && m_ti->isClosable()) {
    if (!m_ti->isWindowModified()) {
      m_tmp = m_ti;
      removeTab(index);
      return true;
    }
    emit sendMessage(tr("Unsaved changes for '%1'!").arg(m_ti->windowTitle()));
  }
  return false;
}

void TabsWidget::setTabChanged(int index) {
  AntiquaCRM::TabsIndex* m_ti = tabIndex(index);
  if (m_ti != nullptr && m_ti->currentPage() == TabsIndex::ViewPage::MainView)
    m_ti->onEnterChanged();
}

void TabsWidget::setTabClosed(int index) {
  if (!m_tabBar->isTabCloseable(index))
    return;

  AntiquaCRM::TabsIndex* m_ti = tabIndex(index);
  if (m_ti->currentPage() != TabsIndex::ViewPage::MainView) {
    setCurrentIndex(index);
    emit sendMessage(tr("The tab cannot be closed in Edit mode!"));
    return;
  }
  removeIndex(index);
}

void TabsWidget::setCurrentTab(const QString& name) {
  QString _id = name.trimmed();
  if (_id.isEmpty() && sender() != nullptr) {
    _id = sender()->objectName();
  }

  if (_id.isEmpty()) {
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << "Missing tab parameter - aborted!";
#endif
    return;
  }

  setCurrentIndex(indexByName(_id));
}

const QIcon TabsWidget::defaultIcon() {
  return AntiquaCRM::antiquaIcon("action-edit-document");
}

int TabsWidget::indexByName(const QString& name) const {
  if (name.isEmpty())
    return -1;

  for (int i = 0; i < m_tabBar->count(); i++) {
    AntiquaCRM::TabsIndex* m_ti = tabIndex(i);
    if (m_ti == nullptr)
      continue;

    if (m_ti->tabIndexId() == name) {
      return i;
    }
  }
  return -1;
}

int TabsWidget::registerTab(AntiquaCRM::TabsIndex* tab) {
  if (tab->tabIndexId().isEmpty()) {
    qWarning("Invalid tab „IndexId“ - rejected!");
    return -1;
  }

  int _index = addTab(tab, tab->windowIcon(), tab->getTitle());
  if (_index >= 0) {
    m_tabBar->setTabCloseable(_index, tab->isClosable());
    tab->onEnterChanged();
  }
  return _index;
}

void TabsWidget::sortTabs() {
  int i = 0;
  foreach (QString _t, p_sorting_tabs) {
    m_tabBar->moveTab(indexByName(_t), i++);
  }
}

AntiquaCRM::TabsIndex* TabsWidget::tabIndex(int index) const {
  return qobject_cast<AntiquaCRM::TabsIndex*>(widget(index));
}

AntiquaCRM::TabsIndex* TabsWidget::tabIndex(const QString& name) const {
  return qobject_cast<AntiquaCRM::TabsIndex*>(widget(indexByName(name)));
}

bool TabsWidget::unloadTabs() {
  for (int t = 0; t < count(); t++) {
    TabsIndex* m_ti = tabIndex(t);
    if (m_ti == nullptr)
      continue;

    if (m_ti->currentPage() != TabsIndex::ViewPage::MainView) {
      setCurrentIndex(t);
      emit sendMessage(tr("'%1' Editor is open!").arg(m_ti->getTitle()));
      return false;
    }

    if (!removeIndex(t))
      qWarning("Tab %s not removed!", qPrintable(m_ti->tabIndexId()));
  }
  return true;
}

} // namespace AntiquaCRM
