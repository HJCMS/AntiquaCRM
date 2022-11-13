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
  const AntiquaCRM::ASettings p_cfg;
  AntiquaCRM::ASqlCore *m_sql;

  /**
   * @brief Statische Kopfdaten Übersetzung!
   */
  const QMap<int, QString> headerList() const;

public Q_SLOTS:
  /**
   * @brief Datensatzabfrage mit Spaltenname und ID
   */
  void setQueryId(const QString &field, qint64 id);

public:
  explicit PurchaseTableModel(AntiquaCRM::ASqlCore *sqlcore,
                              QObject *parent = nullptr);

  /**
   * @brief SQL (INSERT|UPDATE|DELETE).
   */
  // bool submit() override;

  /**
   * @brief Markiert Editierbare Zellen mit einem Icon!
   * @warning Wenn in der QHeaderView, Zellen als nicht Sichtbar markiert
   * werden. Muss mit dieser Liste eine Sicherheitsabfrage gemacht werden!
   * !!! An sonsten wird der komplette Tabellenkopf verschwinden !!!
   * @see PurchaseTable::hideColumns
   */
  static const QList<int> editableColumns();

  /**
   * @brief Wird von removeRow erwartet, daher hier Implementiert.
   */
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;

  /**
   * @brief Übersetzte Darstellung der Tabellenkopfspalten.
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /**
   * @brief Datenfeld anpassungen ...
   */
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PURCHASETABLEMODEL_H
