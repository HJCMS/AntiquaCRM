// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_TABSWIDGET_H
#define ANTIQUACRM_TABS_TABSWIDGET_H

#include <AGlobal>
#include <ASettings>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;
class TabsBar;

/**
 * @class TabsWidget
 * @brief Primary Tab.
 * Class member to handle AntiquaCRM::TabIndex childs.
 *
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsWidget final : public QTabWidget {
  Q_OBJECT

private:
  /**
   * @brief Read properties after insert and set changes.
   */
  virtual void tabInserted(int) override final;

  /**
   * @brief remove from model
   */
  virtual void tabRemoved(int) override final;

  /**
   * @brief close tab by index
   */
  bool removeIndex(int index);

private Q_SLOTS:
  /**
   * @brief Check for TabsIndex::ViewPage
   * @param index - current index
   *
   * If currentIndex() == TabsIndex::ViewPage::MainView call onEnterChanged()
   */
  void setTabChanged(int index);

  /**
   * @brief close tab by index
   * @param index - current index
   */
  void setTabClosed(int index);

protected:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::TabsBar *m_tabBar;

Q_SIGNALS:
  /**
   * @brief Notify status messages
   * @param msg
   *
   * e.g. Current tab is in edit mode ...
   */
  void sendMessage(const QString &msg);

public Q_SLOTS:
  /**
   * @brief overload function for setCurrentIndex(int)
   * @param name = TabsIndex::tabIndexId()
   *
   * This slot search for TabsIndex::tabIndexId() and make it visible!
   * if name isEmpty then it will search for QObject::sender()->objectName()
   */
  void setCurrentTab(const QString &name = QString());

public:
  /**
   * @param parent - parent object
   */
  explicit TabsWidget(QWidget *parent = nullptr);

  /**
   * @brief find tab by QObjectName
   * @param name = objectName()
   */
  int indexByName(const QString &name) const;

  /**
   * @brief overload function for addTab
   * @param tab - object to insert
   */
  int registerTab(AntiquaCRM::TabsIndex *tab);

  /**
   * @brief fallback theme icon
   */
  static const QIcon defaultIcon();

  /**
   * @brief overlaod function for QTabWidget::widget with object_cast
   * @param index
   */
  AntiquaCRM::TabsIndex *tabIndex(int index) const;

  /**
   * @brief overlaod function for tabWithIndex(int)
   * @param name - objectName
   */
  AntiquaCRM::TabsIndex *tabIndex(const QString &name) const;

  /**
   * @brief unload all loaded tab
   */
  bool unloadTabs();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABSWIDGET_H
