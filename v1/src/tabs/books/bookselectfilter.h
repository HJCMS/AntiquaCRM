// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSELECTFILTER_H
#define ANTIQUACRM_BOOKSELECTFILTER_H

#include <QComboBox>
#include <QJsonObject>
#include <QObject>
#include <QMetaType>
#include <QWidget>

class BookSelectFilter final : public QComboBox {
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

  explicit BookSelectFilter(QWidget *parent = nullptr);

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

Q_DECLARE_METATYPE(BookSelectFilter::Filter)

#endif // ANTIQUACRM_BOOKSELECTFILTER_H
