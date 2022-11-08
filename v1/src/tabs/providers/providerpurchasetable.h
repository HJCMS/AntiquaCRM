// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERPURCHASETABLE_H
#define ANTIQUACRM_PROVIDERPURCHASETABLE_H

#include <QContextMenuEvent>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QJsonValue>
#include <QWidget>

/**
 * @class ProviderPurchaseTable
 * @ingroup providers
 * @brief Tabelle für die Bestellartikel anzeigen
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

private Q_SLOTS:
  void prepareOpenArticle();
  void copyIdToClipboard();

protected:
  void addHeaderItem(int, const QString &name);
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void sendCheckArticles();
  void sendOpenArticle(qint64);

public:
  explicit ProviderPurchaseTable(QWidget *parent = nullptr);
  qint64 getSelectedArticleId();
  QTableWidgetItem *createItem(const QJsonValue &data) const;
  const QStringList getArticleIds();
};

#endif // ANTIQUACRM_PROVIDERPURCHASETABLE_H
