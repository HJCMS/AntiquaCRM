// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSPAYEDARTICLES_H
#define ANTIQUACRM_PLUGIN_ORDERSPAYEDARTICLES_H

#include <AntiquaWidgets>
#include <QTableWidget>

class ANTIQUACRM_LIBRARY OrdersPayedArticles final : public QWidget {
  Q_OBJECT

private:
  /**
   * @brief ArticleChanged
   */
  bool ArticleChanged = false;

  /**
   * @brief Tabelle für Artikel einfügen
   */
  QTableWidget *m_table;

Q_SIGNALS:
  /**
   * @brief Signal wenn es in Artikeltabelle änderungen gibt.
   */
  void articleChanged(bool);

public Q_SLOTS:
  /**
   * @brief Tabellen Zeilen zurücksetzen!
   */
  void clearTable();

  /**
   * @brief Importiert einen neuen Tabellen Eintrag
   */
  void insertArticle(const AntiquaCRM::OrderArticleItems &article);

  /**
   * @brief Artikel Tabellen Status::Änderungen setzen!
   */
  void setArticleChanged(bool);

public:
  explicit OrdersPayedArticles(QWidget *parent = nullptr);

  /**
   * @brief Abfrage ob artikel vorhanden sind!
   * Soll verhindern das der Klient Aufträge ohne Inhalt speichert!
   */
  bool isEmpty();

  /**
   * @brief Abfrage auf Artikeltabellen änderungen!
   */
  bool getArticleChanged();

  /**
   * @brief Nach einem INSERT die Tabelle überarbeiten.
   */
  bool setArticleOrderId(qint64 oid);

  /**
   * @brief Bestellartikel aus SQL Abfrage Importieren
   */
  bool importArticles(const QList<AntiquaCRM::OrderArticleItems> &list);

  /**
   * @brief PgSQL-Befehlskette von Tabelle weiterleiten!
   */
  const QStringList getQueryData();
};

#endif // ANTIQUACRM_PLUGIN_ORDERSPAYEDARTICLES_H
