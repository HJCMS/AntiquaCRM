// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PRINTSSTITCHES_FILTERSELECT_H
#define ANTIQUACRM_PRINTSSTITCHES_FILTERSELECT_H

#include <QComboBox>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

/**
 * @brief Inventory Prints Filter selecter
 * @ingroup _prints
 */
class PrintsStitchesFilterSelect final : public QComboBox {
  Q_OBJECT

public:
  enum PSFilter {
    PSF_TITLE = 0x1,    /**< @brief Titelsuche */
    PSF_KEYWORDS = 0x2,  /**< @brief Schlüsselwortsuche */
    PSF_AUTHOR = 0x4,   /**< @brief Autorensuche */
    PSF_ARTICLE = 0x6,      /**< @brief Artikel Nummernsuche */
  };
  Q_DECLARE_FLAGS(PSFilters, PSFilter)

  explicit PrintsStitchesFilterSelect(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_PRINTSSTITCHES_FILTERSELECT_H
