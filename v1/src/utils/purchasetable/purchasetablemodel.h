// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLEMODEL_H
#define ANTIQUACRM_PURCHASETABLEMODEL_H

#include <AntiquaCRM>
#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QObject>
#include <QWidget>

class PurchaseTableColumn;

class PurchaseTableModel final : public QAbstractTableModel {
  Q_OBJECT

private:
  QString currency;
  int p_columns = 11;

  QMap<int, AntiquaCRM::OrderArticleItems> articleRows;

  /**
   * @brief Display Article Media Type
   */
  const QString displayType(int type) const;

  /**
   * @brief Fixed Table Header and Column Definition
   */
  const PurchaseTableColumn tableColumn(int column) const;

  /**
   * @brief List of Editable Columns
   */
  const QList<int> editableColumns() const;

public:
  explicit PurchaseTableModel(QObject *parent = nullptr);

  /**
   * @brief add new Model Data
   */
  bool addArticle(const AntiquaCRM::OrderArticleItems &item);

  /**
   * @brief Clear Table Contents
   */
  void clear();

  /**
   * @brief removeModelData
   * @param index - require QModelIndex(Row,FirstColumn)
   */
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;

  /**
   * @brief set Model Data
   */
  bool addArticles(const QList<AntiquaCRM::OrderArticleItems> &items);

  /**
   * @brief Current row count
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief static column Count
   */
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief Table Header Data
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /**
   * @brief Update Table Cell
   */
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  QString field(const QModelIndex &index) const;

  int columnIndex(const QString &fieldName) const;

  /**
   * @brief Table data
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  /**
   * @brief Edit Flags
   */
  Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // ANTIQUACRM_PURCHASETABLEMODEL_H
