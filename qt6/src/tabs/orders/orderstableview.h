// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSTABLEVIEW_H
#define ANTIQUACRM_PLUGIN_ORDERSTABLEVIEW_H

#include <AntiquaCRM>
#include <QContextMenuEvent>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QPaintEvent>
#include <QTableView>

class OrdersTableModel;
class OrdersTableDelegate;

/**
 * @class OrdersTableView
 * @brief Table view for orders
 *
 * The table creates an overview of all current order items for an order.
 *
 * That is, if you create a new entry, the order number is not yet available!
 *
 * @ingroup PluginOrders
 */
class ANTIQUACRM_LIBRARY OrdersTableView final : public QTableView {
  Q_OBJECT

private:
  QModelIndex p_modelIndex;
  OrdersTableModel *m_model;
  OrdersTableDelegate *m_delegate;
  mutable QStringList sql_cache;

  /**
   * @brief Monitors table view changes
   *
   * QEvent::ModifiedChange and isWindowModified() are monitored.
   *
   * If event is positive, call parentWidget()->setWindowModified(true).
   *
   * @sa OrdersTableView::articleChanged
   */
  void changeEvent(QEvent *) override;

  /**
   * @brief if table is empty, display a notice message.
   */
  void paintEvent(QPaintEvent *) override;

  /**
   * @brief Table row item context menu
   */
  void contextMenuEvent(QContextMenuEvent *) override;

Q_SIGNALS:
  void sendPaymentId(qint64);

private Q_SLOTS:
  /**
   * @brief This Slot checks watched Table Cell changes.
   *
   * It depends on QTableView::dataChanged Signal.
   * With positive values it calls QTableView::setWindowModified and
   * QHeaderView::setStretchLastSection.
   */
  void articleChanged(const QModelIndex &topLeft,
                      const QModelIndex &bottomRight);

  void rowSelected(const QModelIndex &);

  /**
   * @brief Creates SQL-Delete Statement from current selected row.
   */
  void addDeleteQuery();

  /**
   * @brief open article by context menue
   */
  void addArticleQuery();

public Q_SLOTS:
  /**
   * @brief Set Table in readonly mode!
   */
  void setProtected(bool);

  /**
   * @brief Clears the Table content and his buffer.
   */
  void clearContents();

  /**
   * @brief Insert a new Order article.
   */
  void addArticle(const AntiquaCRM::OrderArticleItems &order);

public:
  /**
   * @param parent - parent object
   * @param readOnly - enable/disable ItemDelegation
   */
  explicit OrdersTableView(QWidget *parent = nullptr, bool readOnly = true);

  /**
   * @brief Update field "a_refunds_cost" in all rows
   */
  bool updateRefundCoast(double);

  /**
   * @brief Aktuelle Tabellenzeilenanzahl.
   */
  int rowCount();

  /**
   * @brief is table empty check
   */
  bool isEmpty();

  /**
   * @brief Fieldname list of defined Table cells to be hidden.
   */
  void hideColumns(const QStringList &);

  /**
   * @brief Insert more then one Order Article.
   */
  bool addArticles(const QList<AntiquaCRM::OrderArticleItems> &);

  /**
   * @brief Search for Order Id Cell and update them.
   *
   * An "OrderId" is only generated after the first save and is not yet included
   * in a new entry!
   */
  bool setOrderId(qint64);

  /**
   * @brief Readout buffered SQL-Queries
   */
  const QStringList getSqlQuery();

  /**
   * @brief Check for Unique Article identifier
   * @param articleId - Article Id
   * @return true when already exists
   */
  bool articleExists(qint64 articleId);

  /**
   * @brief Readout Payment Id
   * @param row
   */
  qint64 getPaymentId(int row);

  /**
   * @brief Refundings cost from row.
   * @param row
   */
  double getRefundingCost(int row);

  /**
   * @brief Refundings cost from all rows.
   * @return QMap<Cell::'a_payment_id',Cell::'a_refunds_cost'>
   *
   * This will return all refundings cost with payment id.
   */
  QMap<qint64, double> getRefundingCosts();

  /**
   * @brief Get all entries with Refunded with a cost.
   */
  const QList<AntiquaCRM::OrderArticleItems> getRefundData();
};

#endif // ANTIQUACRM_PLUGIN_ORDERSTABLEVIEW_H
