// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_STITCHESSELECTFILTER_H

#include <AGlobal>
#include <QComboBox>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY StitchesSelectFilter : public QComboBox {
  Q_OBJECT

public:
  enum Filter {
    STITCHES_TITLE_AUTHOR = 0,  /**< @brief Büchersuche */
    STITCHES_TITLE_KEYWORD = 1, /**< @brief Bucher + Schlüsselwortsuche */
    STITCHES_AUTHORS = 2,       /**< @brief Autorensuche */
    STITCHES_ARTICLE_ID = 3,    /**< @brief Artikel Nummernsuche */
    STITCHES_STORAGE = 4,       /**< @brief Lager Suche */
    STITCHES_PUBLISHER = 5      /**< @brief Verlag/Hersteller Suche */
  };

  explicit StitchesSelectFilter(QWidget *parent = nullptr);

  StitchesSelectFilter::Filter currentFilter(int index = -1);

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

#endif // ANTIQUACRM_PLUGIN_STITCHESSELECTFILTER_H
