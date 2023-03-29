// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_TABLECONTEXTMENU_H
#define ANTIQUACRM_WIDGETS_TABLECONTEXTMENU_H

#include <AntiquaInput>
#include <QAction>
#include <QIcon>
#include <QList>
#include <QMenu>
#include <QModelIndex>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY TableContextMenu final : public QMenu {
  Q_OBJECT

public:
  enum Actions {
    Open = 0,   /**< @brief Open Item Action */
    Create = 1, /**< @brief Create new Item Action */
    Copy = 2,   /**< @brief Copy Item Action */
    Delete = 3, /**< @brief Delete Item Action */
    Order = 4,  /**< @brief Create/Add Order action */
    Undo = 5,   /**< @brief Undo action */
    Custom = 6  /**< @brief Custom Item action */
  };

  /**
   * @brief TableContextMenu
   * @param index  - Selected Table ModelIndex
   * @param rowCount - Parent Table rowsCount
   * @param parent - Table
   */
  explicit TableContextMenu(const QModelIndex &index, qint64 rowCount = 0,
                            QWidget *parent = nullptr);

private:
  /**
   * @brief Selected Table ModelIndex
   */
  QModelIndex p_index;

  /**
   * @brief Current RowCount from Table
   */
  qint64 p_rows;

  /**
   * @brief Actions with ModelIndex
   * Actions from this list requires a Signal with ModelIndex
   */
  QList<QAction *> p_modelList;

private Q_SLOTS:
  void triggerAction(QAction *);

Q_SIGNALS:
  void sendAction(TableContextMenu::Actions, const QModelIndex &);
  void sendCreate();  /**< @brief Create new Entry */
  void sendRefresh(); /**< @brief Reafresh View */

public:
  /**
   * @brief get Icon from private Icontheme
   */
  const QIcon getIcon(const QString &) const;

  /**
   * @brief add Open by ModelIndex Action
   * @note sendAction(TableContextMenu::Actions::Open,index);
   */
  QAction *addOpenAction(const QString &title);

  /**
   * @brief add Create Action
   * @note sendCreate();
   */
  QAction *addCreateAction(const QString &title);

  /**
   * @brief add create Order by ModelIndex Action
   * @note sendAction(TableContextMenu::Actions::Order,index);
   */
  QAction *addOrderAction(const QString &title);

  /**
   * @brief add Delete by ModelIndex Action
   * @note sendAction(TableContextMenu::Actions::Delete,index);
   */
  QAction *addDeleteAction(const QString &title);

  /**
   * @brief add Copy by ModelIndex Action
   * @note sendAction(TableContextMenu::Actions::Copy,index);
   */
  QAction *addCopyAction(const QString &title);

  /**
   * @brief add Reload Current TableView
   * @note sendRefresh();
   */
  QAction *addReloadAction(const QString &title);

  /**
   * @brief add Reload Current TableView
   * @note sendAction(TableContextMenu::Actions::Undo,index);
   */
  QAction *addUndoAction(const QString &title);

  /**
   * @brief add Custom TableView action
   * @note sendAction(TableContextMenu::Actions::Custom,index);
   */
  QAction *addCustomAction(const QIcon &icon, const QString &title);
};

} // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::TableContextMenu::Actions);

#endif // ANTIQUACRM_WIDGETS_TABLECONTEXTMENU_H
