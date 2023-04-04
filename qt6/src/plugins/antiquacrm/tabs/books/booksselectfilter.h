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
#include <QMetaType>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY BooksSelectFilter : public QComboBox {
  Q_OBJECT

public:
  enum Filter {
    BooksAuthor = 0x1,  /**< @brief Büchersuche */
    BooksKeyword = 0x2, /**< @brief Bucher + Schlüsselwortsuche */
    Authors = 0x4,      /**< @brief Autorensuche */
    ISBN = 0x8,         /**< @brief ISBN Suche */
    ArticleId = 0x10,   /**< @brief Artikel Nummernsuche */
    Storage = 0x20,     /**< @brief Lager Suche */
    Publisher = 0x40    /**< @brief Verlag/Hersteller Suche */
  };
  Q_DECLARE_FLAGS(Filters, Filter)

  explicit BooksSelectFilter(QWidget *parent = nullptr);

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

Q_DECLARE_METATYPE(BooksSelectFilter::Filter)

#endif // ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H
