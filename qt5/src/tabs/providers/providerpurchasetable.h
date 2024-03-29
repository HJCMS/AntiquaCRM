// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PROVIDERPURCHASETABLE_H
#define ANTIQUACRM_PROVIDERPURCHASETABLE_H

#include <QContextMenuEvent>
#include <QIcon>
#include <QJsonValue>
#include <QObject>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

/**
 * @ingroup _providers
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
class ProviderPurchaseTable final : public QTableWidget {
  Q_OBJECT

private:
  const QIcon pic(const QString &name) const;

private Q_SLOTS:
  void prepareOpenArticle();
  void preparePrintCard();
  void copyIdToClipboard();

protected:
  void addHeaderItem(int, const QString &name);
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void sendCheckArticles();
  void sendOpenArticle(qint64);
  void sendPrintCard(qint64);

public Q_SLOTS:
  void setArticleStatus(const QString &article, bool available = false);

public:
  explicit ProviderPurchaseTable(QWidget *parent = nullptr);
  qint64 getSelectedArticleId();
  QTableWidgetItem *createItem(const QJsonValue &data) const;
  const QStringList getArticleIds();
};

#endif // ANTIQUACRM_PROVIDERPURCHASETABLE_H
