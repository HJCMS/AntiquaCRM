// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvfilterselect.h"

CDVFilterSelect::CDVFilterSelect(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(":icons/view_search.png");
  int col = 0;
  insertItem(col, icon, tr("Artists"), CDVFilter::CDV_AUTHOR);
  setItemData(col++, tr("Search with Artistnames"), Qt::ToolTipRole);

  insertItem(col, icon, tr("Title"), CDVFilter::CDV_TITLE);
  setItemData(col++, tr("Search with Title"), Qt::ToolTipRole);

  insertItem(col, icon, tr("Genres/Keywords"), CDVFilter::CDV_KEYWORDS);
  setItemData(col++, tr("Search with Keyword and Genre"), Qt::ToolTipRole);

  insertItem(col, icon, tr("Article Id"), CDVFilter::CDV_ARTICLE);
  setItemData(col++, tr("Multiple searches separated by commas"),
              Qt::ToolTipRole);

  insertItem(col, icon, tr("Barcode"), CDVFilter::CDV_ISBNEAN);
  setItemData(col++, tr("Search with Barcode"), Qt::ToolTipRole);
}

const QJsonObject CDVFilterSelect::getFilter(int index) {
  QJsonObject obj;
  switch (static_cast<CDVFilter>(itemData(index, Qt::UserRole).toInt())) {
  case CDV_AUTHOR: /**< @brief Autorensuche */
    obj.insert("search", QJsonValue("artists"));
    obj.insert("fields", QJsonValue("cv_author"));
    break;

  case CDV_TITLE: /**< @brief Titelsuche */
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("cv_title"));
    break;

  case CDV_KEYWORDS: /**< @brief Schlüsselwortsuche */
    obj.insert("search", QJsonValue("genres_keywords"));
    obj.insert("fields", QJsonValue("cv_keyword,cv_description"));
    break;

  case CDV_ARTICLE: /**< @brief Artikel Nummernsuche */
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("cv_id"));
    break;

  case CDV_ISBNEAN: /**< @brief Barcode */
    obj.insert("search", QJsonValue("barcode"));
    obj.insert("fields", QJsonValue("cv_eangtin"));
    break;

  default:
    obj.insert("search", QJsonValue("artists"));
    obj.insert("fields", QJsonValue("cv_author"));
    break;
  };

  return obj;
}
