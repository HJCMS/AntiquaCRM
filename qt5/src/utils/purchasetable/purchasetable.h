// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PURCHASETABLE_H
#define ANTIQUACRM_PURCHASETABLE_H

#include <AntiquaCRM>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QPaintEvent>
#include <QTableView>

class PurchaseTableModel;
class PurchaseTableDelegate;

/**
 * @ingroup _purchasetable
 * @brief Tabelle Artikeleinkäufe
 *
 * @class PurchaseTable
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
   * @brief Auf Änderungen prüfen
   */
  void changeEvent(QEvent *) override;

  /**
   * @brief Infos zum Tabellinhalt
   */
  void paintEvent(QPaintEvent *) override;

  /**
   * @brief Menü für Zeilen entfernen!
   */
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  /**
   * @brief articleChanged
   */
  void articleChanged(const QModelIndex &, const QModelIndex &);

  /**
   * @brief Ausgewählte Zeile entfernen!
   */
  void removeArticle();

Q_SIGNALS:
  void sendTableModified(bool);

public Q_SLOTS:
  /**
   * @brief Tabelleninhalt leeren
   */
  void clearTable();

  /**
   * @brief Einen neuen Artikel in die Tabelle aufnehmen!
   */
  void addOrderArticle(const AntiquaCRM::OrderArticleItems &);

public:
  /**
   * @brief Einkaufs Tabelle
   * @param parent
   * @param readOnly - Wenn aktiviert wird PurchaseTableDelegate deaktiviert!
   */
  explicit PurchaseTable(QWidget *parent = nullptr, bool readOnly = true);

  /**
   * @brief Tabellenzeilen zurück geben!
   */
  int rowCount();

  /**
   * @brief Tabellenspalten ausblenden!
   * @warning Wenn man Tabellenspalten ausblendet, die Editiert werden,
   * verschwindet der komplette Tabellen Header!
   * @see PurchaseTableModel::editableColumns
   */
  void hideColumns(const QStringList &);

  /**
   * @brief Leert die Tabelle und importiert alle Artikel aus der Liste.
   */
  bool setOrderArticles(const QList<AntiquaCRM::OrderArticleItems> &);

  /**
   * @brief Fehlende Article Order ID einfügen!
   */
  bool setArticleOrderId(qint64);

  /**
   * @brief Create SQL UPDATE | INSERT Statements.
   * Diese Funktion erstellt aus den Vorhandenen Tabellen einträgen den SQL Code
   * für INSERT und UPDATE.
   */
  const QStringList getSqlQuery();

  /**
   * @brief Abfrage der Zahlungs Nummer auf Zeilenummer.
   */
  qint64 getArticlePaymentId(int);
};

#endif // ANTIQUACRM_PURCHASETABLE_H
