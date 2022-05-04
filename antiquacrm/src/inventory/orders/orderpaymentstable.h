// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERPAYMENTSTABLE_H
#define INVENTORY_ORDERPAYMENTSTABLE_H

#include <QObject>
#include <QTableWidget>
#include <QVariant>
#include <QWidget>

/**
 * @ingroup InventoryOrder
 * @class OrderPaymentsTable
 */
class OrderPaymentsTable : public QTableWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Einen neuen QTableWidgetItem erstellen.
   */
  void addHeaderItem(int, const QString &name, const QString &tip);

public:
  /**
   * @brief OrderPaymentsTable
   */
  OrderPaymentsTable(QWidget *parent = nullptr);

  int getArticleId(int row) const;
};

#endif // INVENTORY_ORDERPAYMENTSTABLE_H
