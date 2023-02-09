// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVFILTERSELECT_H
#define ANTIQUACRM_CDVFILTERSELECT_H

#include <QComboBox>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

class CDVFilterSelect final : public QComboBox {
  Q_OBJECT

public:
  enum CDVFilter {
    CDV_AUTHOR = 0x1,   /**< @brief Künstlersuche */
    CDV_KEYWORDS = 0x2, /**< @brief Schlüsselwortsuche */
    CDV_TITLE = 0x4,    /**< @brief Titelsuche */
    CDV_ARTICLE = 0x6,  /**< @brief Artikel Nummernsuche */
    CDV_ISBNEAN = 0x8,  /**< @brief ISBN/EAN Suche */
  };
  Q_DECLARE_FLAGS(CDVFilters, CDVFilter)

  explicit CDVFilterSelect(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_CDVFILTERSELECT_H
