// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLE_H
#define ANTIQUACRM_PURCHASETABLE_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QContextMenuEvent>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QPair>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>

class PurchaseTableModel;
class PurchaseTableDelegate;

class PurchaseTable final : public QTableView {
  Q_OBJECT

private:
  PurchaseTableModel *m_model;
  PurchaseTableDelegate *m_delegate;
  QHeaderView *m_headerView;
  bool removeRow(int row);

public Q_SLOTS:
  void clearTable();

  /**
   * @brief Add a new Order Article to the Table
   */
  void addOrderArticle(const AntiquaCRM::OrderArticleItems &item);

public:
  explicit PurchaseTable(QWidget *parent = nullptr, bool readOnly = true);

  /**
   * @brief Set Visibility for Columns
   */
  void hideColumns(const QList<int> &list);

  /**
   * @brief Import OrderArticles
   * @note Before insert the Table clear the Contents!
   */
  bool setOrderArticles(const QList<AntiquaCRM::OrderArticleItems> &items);

  /**
   * @brief get current Order Articles from table
   */
  const QStringList getQueryArticles();
};

#endif // ANTIQUACRM_PURCHASETABLE_H
