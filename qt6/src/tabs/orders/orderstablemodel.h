// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSTABLEMODEL_H
#define ANTIQUACRM_PLUGIN_ORDERSTABLEMODEL_H

#include <AntiquaCRM>
#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QObject>
#include <QWidget>

/**
 * @class OrdersTableModel
 * @brief Order Articles Table model
 *
 * @ingroup PluginOrders
 */
class OrdersTableModel final : public QAbstractTableModel {
  Q_OBJECT

private:
  QString currency;           /**< Currency symbol */
  int vat_normal;             /**< Default Tax value */
  int vat_reduced;            /**< Reduced Tax value */
  int p_columns = 13;         /**< Will overridden in addArticles */
  int max_string_length = 79; /**< Reduces the Title Display size to ... */

  /**
   * @brief All article items
   */
  mutable QMap<int, AntiquaCRM::OrderArticleItems> articles;

  /**
   * @brief Convert double to Currency with System locales
   */
  const QString displayPrice(double) const;

  /**
   * @brief Update item with index from \b articles.
   * @param index - Current row
   * @param data  - OrderArticleItems to change,
   *                data must equal to column count!
   *
   * This will emitted dataChanged(QModelIndex,QModelIndex) on success!
   */
  bool updateRow(int index, const AntiquaCRM::OrderArticleItems& data);

public Q_SLOTS:
  /**
   * @brief Delete the complete table content without the header data!
   */
  void clearContents();

public:
  enum TaxType { NORMAL = 0x00, REDUCED = 0x01 };

  explicit OrdersTableModel(QObject* parent = nullptr);

  const QModelIndex parentIndex() const;

  /**
   * @brief Returns the size of payment articles.
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief The number of cells is always the same!
   * @see p_columns
   */
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Return table headers.
   * @note In the case of the vertical header data, the display of the number of
   * lines is corrected to the human logical value!
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /**
   * @brief Cell data of a table row
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /**
   * @brief Write data to cells...
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  /**
   * @brief Processing indicator
   * @note default (Qt::ItemIsSelectable | Qt::ItemIsEnabled)
   * Editable cells have editableColumns():
   *  @li Qt::ItemIsSelectable
   *  @li Qt::ItemIsEnabled
   *  @li Qt::ItemIsEditable
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * @brief Remove an entry from articles!
   * @warning !!! NOT A STANDARD AS SPECIFIED IN THE DOCUMENTATION !!!
   *
   * If I work with \i beginRemoveRows and \i endRemoveRows as described in the
   * documentation!
   *
   * If table contents are not processed correctly with a QMap. For example,
   * with a subsequent rowCount(), the old value is returned even though the
   * size of \b articles has changed. Because the number of orders rarely
   * exceeds 10, it is easier to empty the table and re-enter all the data,
   * which works.
   */
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  /**
   * @brief Returns the SQL table field name by cell index.
   */
  const QString field(const QModelIndex& index) const;

  /**
   * @brief Returns the cell index using the SQL table field name.
   */
  int columnIndex(const QString& fieldName) const;

  /**
   * @brief Includes the SQL table field names and header translations!
   * @note The field names must be defined in the @ref AProviderOrder!
   * @warning Otherwise a misspelled data type will return!
   */
  static const AntiquaCRM::ATableHeaderColumn headerColumn(int column);

  /**
   * @brief Add a new item order.
   */
  bool addArticle(const AntiquaCRM::OrderArticleItems& item);

  /**
   * @brief Add a new item orders list.
   */
  bool addArticles(const QList<AntiquaCRM::OrderArticleItems>& items);

  /**
   * @brief Returns the current VAT value for this Article.
   */
  qreal taxValue(OrdersTableModel::TaxType type) const;

  /**
   * @brief Returns the list of editable cells!
   */
  const QList<int> editableColumns() const;

  /**
   * @brief Returns the type of the current article as text.
   * @sa AntiquaCRM::ArticleType
   */
  static const QString articleType(int type);
};

#endif // ANTIQUACRM_PLUGIN_ORDERSTABLEMODEL_H
