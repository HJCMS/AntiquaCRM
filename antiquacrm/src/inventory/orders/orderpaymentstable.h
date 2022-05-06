// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERPAYMENTSTABLE_H
#define INVENTORY_ORDERPAYMENTSTABLE_H

#include <QContextMenuEvent>
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

private Q_SLOTS:
  void removeTableItem();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  /**
   * @brief Signal Tabellenzeile entfernen
   */
  void s_removeTableRow(int row);

public:
  /**
   * @brief OrderPaymentsTable
   */
  OrderPaymentsTable(QWidget *parent = nullptr);

  int getArticleId(int row) const;
};

#endif // INVENTORY_ORDERPAYMENTSTABLE_H
