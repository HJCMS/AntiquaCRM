// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsstitchesfilterselect.h"

PrintsStitchesFilterSelect::PrintsStitchesFilterSelect(QWidget *parent)
    : QComboBox{parent} {
  setWhatsThis(
      tr("With this button you can optimize your search with some filters."));
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(":icons/view_search.png");
  int col = 0;
  insertItem(col, icon, tr("Title"), PSFilter::PSF_TITLE);
  setItemData(col++, tr("Search in Prints and Stitches by Title"),
              Qt::ToolTipRole);

  insertItem(col, icon, tr("Article Id"), PSFilter::PSF_ARTICLE);
  setItemData(col++, tr("Multiple searches separated by commas"),
              Qt::ToolTipRole);

  insertItem(col, icon, tr("Keywords"), PSFilter::PSF_KEYWORDS);
  setItemData(col++, tr("Search in Prints and Stitches by Kewords"),
              Qt::ToolTipRole);

  insertItem(col, icon, tr("Author"), PSFilter::PSF_AUTHOR);
  setItemData(col++, tr("Search for Prints and Stitches authors"),
              Qt::ToolTipRole);
}

const QJsonObject PrintsStitchesFilterSelect::getFilter(int index) {
  QJsonObject obj;
  switch (static_cast<PSFilter>(itemData(index, Qt::UserRole).toInt())) {
  case PSF_TITLE: /**< @brief Titelsuche */
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ip_title"));
    break;

  case PSF_KEYWORDS: /**< @brief Schlüsselwortsuche */
    obj.insert("search", QJsonValue("keywords"));
    obj.insert("fields", QJsonValue("ip_designation,ip_keyword"));
    break;

  case PSF_AUTHOR: /**< @brief Autorensuche */
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("ip_author"));
    break;

  case PSF_ARTICLE: /**< @brief Artikel Nummernsuche */
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("ip_id"));
    break;

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ip_title"));
    break;
  };

  return obj;
}
