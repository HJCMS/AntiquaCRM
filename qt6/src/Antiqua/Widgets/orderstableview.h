// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_ORDERSTABLEVIEW_H
#define ANTIQUACRM_WIDGETS_ORDERSTABLEVIEW_H

#include <AntiquaCRM>
#include <QContextMenuEvent>
#include <QModelIndex>
#include <QObject>
#include <QPaintEvent>
#include <QTableView>

namespace AntiquaCRM {

class TableHeader;
class OrdersTableModel;
class OrdersTableDelegate;

class ANTIQUACRM_LIBRARY OrdersTableView final : public QTableView {
  Q_OBJECT

private:
  QModelIndex p_modelIndex;
  OrdersTableModel *m_model;
  OrdersTableDelegate *m_delegate;
  QStringList sql_cache;
  void changeEvent(QEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void articleChanged(const QModelIndex &, const QModelIndex &);
  void removeArticle();

public Q_SLOTS:
  void clearContents();
  void addArticle(const AntiquaCRM::OrderArticleItems &);

Q_SIGNALS:
  void sendTableModified(bool);

public:
  /**
   * @param parent - parent object
   * @param readOnly - enable/disable ItemDelegation
   */
  explicit OrdersTableView(QWidget *parent = nullptr, bool readOnly = true);

  int rowCount();

  void hideColumns(const QStringList &);

  bool setOrderArticles(const QList<AntiquaCRM::OrderArticleItems> &);

  bool setArticleOrderId(qint64);

  const QStringList getSqlQuery();

  qint64 getPaymentId(int);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_ORDERSTABLEVIEW_H
