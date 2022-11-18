// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLE_H
#define ANTIQUACRM_PURCHASETABLE_H

#include <AntiquaCRM>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QTableView>

class PurchaseTableModel;
class PurchaseTableDelegate;

/**
 * @brief Tabelle Artikeleinkäufe
 * @section widgets
 * Es werden hier aus Performance gründen keine SQL Abfragen gemacht!
 * Der Grund hierfür ist, das AntiquaCRM primär für Remote Desktop erstellt
 * wird. Zu viele Hintergrundabfragen können je nach Netzwerkleistung zu
 * problemen führen!
 */
class PurchaseTable final : public QTableView {
  Q_OBJECT

private:
  PurchaseTableModel *m_model;
  PurchaseTableDelegate *m_delegate;
  QHeaderView *m_headerView;
  QModelIndex p_modelIndex;
  QString sqlToRemoveCache;

  /**
   * @brief Menü für Zeilen entfernen!
   */
  void contextMenuEvent(QContextMenuEvent *event) override;

private Q_SLOTS:
  /**
   * @brief articleChanged
   */
  void articleChanged(const QModelIndex &topLeft,
                      const QModelIndex &bottomRight);

  /**
   * @brief Ausgewählte Zeile entfernen!
   */
  void removeArticle();

public Q_SLOTS:
  /**
   * @brief Tabelleninhalt leeren
   */
  void clearTable();

  /**
   * @brief Einen neuen Artikel in die Tabelle aufnehmen!
   */
  void addOrderArticle(const AntiquaCRM::OrderArticleItems &item);

public:
  /**
   * @brief Einkaufs Tabelle
   * @param readOnly - Wenn aktiviert wird PurchaseTableDelegate deaktiviert!
   */
  explicit PurchaseTable(QWidget *parent = nullptr, bool readOnly = true);

  /**
   * @brief Tabellenspalten ausblenden!
   * @warning Wenn man Tabellenspalten ausblendet, die Editiert werden,
   * verschwindet der komplette Tabellen Header!
   * @see PurchaseTableModel::editableColumns
   */
  void hideColumns(const QStringList &list);

  /**
   * @brief Leert die Tabelle und importiert alle Artikel aus der Liste.
   */
  bool setOrderArticles(const QList<AntiquaCRM::OrderArticleItems> &items);

  /**
   * @brief Fehlende Article Order ID einfügen!
   */
  bool setArticleOrderId(qint64 oid);

  /**
   * @brief Create SQL UPDATE | INSERT Statements.
   * Diese Funktion erstellt aus den Vorhandenen Tabellen einträgen den SQL Code
   * für INSERT und UPDATE.
   */
  const QStringList getSqlQuery();
};

#endif // ANTIQUACRM_PURCHASETABLE_H
