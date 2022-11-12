// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLEMODEL_H
#define ANTIQUACRM_PURCHASETABLEMODEL_H

#include <AntiquaCRM>
#include <QIcon>
#include <QMap>
#include <QObject>
#include <QSqlTableModel>
#include <QWidget>

class PurchaseTableModel final : public QSqlTableModel {
  Q_OBJECT

private:
  QString queryHistory;
  const AntiquaCRM::ASettings p_cfg;
  AntiquaCRM::ASqlCore *m_sql;
  const QMap<int, QString> headerList() const;

public Q_SLOTS:
  void reload();
  void setQueryId(const QString &field, qint64 id);

public:
  explicit PurchaseTableModel(AntiquaCRM::ASqlCore *sqlcore,
                              QObject *parent = nullptr);

  /**
   * @brief Insert a new row
   */
  bool insertRow(qint64 orderId, const AntiquaCRM::OrderArticleItems &items);

  /**
   * @brief Markiert Editierbare Zellen mit einem Icon!
   * @warning Wenn in der QHeaderView, Zellen als nicht Sichtbar markiert werden.
   * Muss mit dieser Liste eine Sicherheitsabfrage gemacht werden!
   * !!! An sonsten wird der komplette Tabellenkopf verschwinden !!!
   * @see PurchaseTable::hideColumns
   */
  static const QList<int> editableColumns();

  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PURCHASETABLEMODEL_H
