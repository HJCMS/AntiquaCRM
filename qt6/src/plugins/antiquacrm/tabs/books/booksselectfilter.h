// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H

#include <AGlobal>
#include <QComboBox>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY BooksSelectFilter : public QComboBox {
  Q_OBJECT

public:
  enum Filter {
    BOOK_TITLE_AUTHOR = 0,  /**< @brief Büchersuche */
    BOOK_TITLE_KEYWORD = 1, /**< @brief Bucher + Schlüsselwortsuche */
    BOOK_AUTHORS = 2,       /**< @brief Autorensuche */
    BOOK_ISBN = 3,          /**< @brief ISBN Suche */
    BOOK_ARTICLE_ID = 4,    /**< @brief Artikel Nummernsuche */
    BOOK_STORAGE = 5,       /**< @brief Lager Suche */
    BOOK_PUBLISHER = 6      /**< @brief Verlag/Hersteller Suche */
  };

  explicit BooksSelectFilter(QWidget *parent = nullptr);

  BooksSelectFilter::Filter currentFilter(int index = -1);

  /**
   * @brief Nehme aktuellen Suchfilter
   * @note Wenn -1 dann wird currentIndex() verwendet!
   * @code
   *  QJsonObject(
   *    "search" => QJSonValue().toString()
   *    "fields" => QJSonValue().toString(),
   *  );
   * @endcode
   */
  const QJsonObject getFilter(int index = -1);
};

#endif // ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H
