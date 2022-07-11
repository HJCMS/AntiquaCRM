// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PURCHASERORDERTABLE_H
#define ANTIQUA_PURCHASERORDERTABLE_H

#include <QContextMenuEvent>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

#include "antiqua_global.h"

namespace Antiqua {
/**
 * @class Antiqua::PurchaserOrderTable
 * @ingroup Antiqua Plugin Interface
 * @brief Tabelle für die Bestellartikel anzeigen
 * Die Standardformatierung der Zellen ist Text und alle Zellen sind
 * auf nur lesen gestellt. Die Tabelle dient Primär zur Bestellungsanzeige.
 * Es werden die Artikel Nr. mit Antiqua::InterfaceWidget::getArticleIds(column)
 * ausgelesen. Sie besteht aus 5 Spalten welche wie folgt vergeben sind:
 * @code
 *  --------------------------------------------------------
 *  |        0         |      1     |   2   |   3   |   4  |
 *  --------------------------------------------------------
 *  | Dienstleister ID | Artikel ID | Menge | Preis | Info |
 *  --------------------------------------------------------
 * @endcode
 */
class ANTIQUACORE_EXPORT PurchaserOrderTable final : public QTableWidget {
  Q_OBJECT

private Q_SLOTS:
  void findSelectedArticleId();
  void copyIdToClipboard();

protected:
  void addHeaderItem(int, const QString &name);
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void inspectArticle(int);
  void findArticleNumbers();

public:
  explicit PurchaserOrderTable(QWidget *parent = nullptr);
  QTableWidgetItem *createItem(const QString &title) const;
};
};     // namespace Antiqua
#endif // ANTIQUA_PURCHASERORDERTABLE_H
