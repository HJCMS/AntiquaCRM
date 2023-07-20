// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_ORDERSTABLEDELEGATE_H
#define ANTIQUACRM_PRIVATE_ORDERSTABLEDELEGATE_H

#include <AntiquaWidgets>
#include <QAbstractItemModel>
#include <QByteArray>
#include <QItemDelegate>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY OrdersTableDelegate final : public QItemDelegate {
  Q_OBJECT

private:
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
  EditorProperties config;

public:
  explicit OrdersTableDelegate(QObject *parent = nullptr);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_ORDERSTABLEDELEGATE_H
