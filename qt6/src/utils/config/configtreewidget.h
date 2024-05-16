// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_TREEWIDGET_H
#define ANTIQUACRM_DIALOGS_TREEWIDGET_H

#include <QJsonObject>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>

/**
 * @class ConfigTreeWidget
 * @brief Configurations Tree
 */
class ConfigTreeWidget final : public QTreeWidget {
  Q_OBJECT

private:
  QTreeWidgetItem *m_mainIndex;     /**< @brief Category: general */
  QTreeWidgetItem *m_tabsIndex;     /**< @brief Category: config tabs */
  QTreeWidgetItem *m_providerIndex; /**< @brief Category: provider tabs */

  /**
   * @brief Initial Child Item from parent with default properties
   * @param parent - category item object
   */
  QTreeWidgetItem *childItem(QTreeWidgetItem *parent) const;

  /**
   * @brief insert child item with JsonObject properties
   * @param main - parent category item object
   * @param page - page index
   */
  void addChild(QTreeWidgetItem *main, int page, const QJsonObject &);

private Q_SLOTS:
  /**
   * @brief internal handle to QTreeWidgetItem::itemClicked
   */
  void setItemSelected(QTreeWidgetItem *, int);

Q_SIGNALS:
  /**
   * @brief send open page by index, emitted by setItemSelected
   */
  void sendPageIndex(int);

  /**
   * @brief open root path section
   */
  void sendConfigGroup(const QString &section);

public Q_SLOTS:
  /**
   * @brief append entry into general Settings Category
   * @param page - page index
   * @param jobj - JsonObject
   * @see AntiquaCRM::PluginConfigWidget::getMenuEntry()
   */
  void addGeneral(int page, const QJsonObject &jobj);

  /**
   * @brief append plugin config entry to Tabs category
   * @param page - page index
   * @param jobj - JsonObject
   * @see AntiquaCRM::PluginConfigWidget::getMenuEntry()
   */
  void addTabPlugin(int page, const QJsonObject &jobj);

  /**
   * @brief append plugin config entry to Provider category
   * @param page - page index
   * @param jobj - JsonObject
   * @see AntiquaCRM::PluginConfigWidget::getMenuEntry()
   */
  void addProviderPlugin(int page, const QJsonObject &jobj);

public:
  explicit ConfigTreeWidget(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DIALOGS_TREEWIDGET_H
