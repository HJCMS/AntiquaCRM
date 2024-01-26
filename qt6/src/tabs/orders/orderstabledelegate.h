// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSTABLEDELEGATE_H
#define ANTIQUACRM_PLUGIN_ORDERSTABLEDELEGATE_H

#include <AntiquaWidgets>
#include <QAbstractItemModel>
#include <QByteArray>
#include <QItemDelegate>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QWidget>

/**
 * @class OrdersTableDelegate
 * @brief Order articles cell item delegate
 *
 * @ingroup PluginOrders
 */
class ANTIQUACRM_LIBRARY OrdersTableDelegate final : public QItemDelegate {
  Q_OBJECT

public:
  /**
   * @brief Input properties
   */
  struct EditorProperties {
    qreal minPrice;
    qreal maxPrice;
    int minCount;
    int maxCount;
    int maxInteger;
    QByteArray currency;
  };

  /**
   * @brief parent - parent object
   */
  explicit OrdersTableDelegate(QObject *parent = nullptr);

  /**
   * @brief Change minimum sale price
   * @param price
   */
  void setMinPrice(double price);

  void setProperties(const OrdersTableDelegate::EditorProperties &);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;

private:
  EditorProperties config;
};

#endif // ANTIQUACRM_PLUGIN_ORDERSTABLEDELEGATE_H
