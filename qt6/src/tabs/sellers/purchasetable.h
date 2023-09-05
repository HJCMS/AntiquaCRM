// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_PURCHASETABLE_H
#define ANTIQUACRM_PLUGIN_SELLERS_PURCHASETABLE_H

#include <AGlobal>
#include <QContextMenuEvent>
#include <QJsonValue>
#include <QTableWidget>
#include <QTableWidgetItem>

/**
 * @class PurchaseTable
 * @brief Tabelle für die Bestellartikel anzeigen
 *
 * @class ProviderPurchaseTable
 * Die Standardformatierung der Zellen ist Text und alle Zellen sind
 * auf nur lesen gestellt. Die Tabelle dient Primär zur Bestellungsanzeige.
 * Sie besteht aus 5 Spalten welche wie folgt vergeben sind:
 * @code
 *  --------------------------------------------------------
 *  |        0         |      1     |   2   |   3   |   4  |
 *  --------------------------------------------------------
 *  | Dienstleister ID | Artikel ID | Menge | Preis | Info |
 *  --------------------------------------------------------
 * @endcode
 */
class ANTIQUACRM_LIBRARY PurchaseTable final : public QTableWidget {
  Q_OBJECT

private Q_SLOTS:
  void prepareOpenArticle();
  void copyIdToClipboard();
  void createPrintingCard();

protected:
  void addHeaderItem(int, const QString &name);
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void sendCheckArticles();
  void sendOpenArticle(qint64);

public Q_SLOTS:
  void setArticleStatus(const QString &article, bool available = false);

public:
  explicit PurchaseTable(QWidget *parent = nullptr);
  qint64 getSelectedArticleId();
  QTableWidgetItem *createItem(const QJsonValue &data) const;
  const QStringList getArticleIds();
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_PURCHASETABLE_H
