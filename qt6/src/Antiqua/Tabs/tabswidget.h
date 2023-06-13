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
 * @brief Toplevel Tab widget class
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsWidget final : public QTabWidget {
  Q_OBJECT

private:
  virtual void tabInserted(int) override final;

private Q_SLOTS:
  void setEnterChanged(int);

protected:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::TabsBar *m_tabBar;

Q_SIGNALS:
  void sendMessage(const QString &);

public Q_SLOTS:
  /**
   * @brief Search Tab with objectName and call setCurrentIndex!
   */
  void setCurrentTab(const QString &objname);

  /**
   * @brief overlaod function for setCurrentTab
   */
  void setViewTab();

public:
  /**
   * @param parent - parent object
   */
  explicit TabsWidget(QWidget *parent = nullptr);

  /**
   * @brief find tab by QObjectName
   */
  int indexByName(const QString &objname);

  /**
   * @brief overload function for addTab
   * @param tab
   * @param title
   */
  int registerTab(AntiquaCRM::TabsIndex *tab, const QString &title);

  /**
   * @brief fallback theme icon
   */
  static const QIcon defaultIcon();

  /**
   * @brief overlaod function for QTabWidget::widget with object_cast
   */
  AntiquaCRM::TabsIndex *tabWithIndex(int);

  /**
   * @brief overlaod function for tabWithIndex(int)
   */
  AntiquaCRM::TabsIndex *tabWithName(const QString &);

  /**
   * @brief unload all loaded tab
   */
  bool unloadTabs();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABSWIDGET_H
