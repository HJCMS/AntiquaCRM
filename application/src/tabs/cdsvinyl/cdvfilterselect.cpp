// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvfilterselect.h"

CDVFilterSelect::CDVFilterSelect(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(":icons/view_search.png");
  int col = 0;
  insertItem(col, icon, tr("Title"), CDVFilter::CDV_TITLE);
  setItemData(col++, tr("Search for CD/Vinyl by Title"), Qt::ToolTipRole);

  insertItem(col, icon, tr("Article Id"), CDVFilter::CDV_ARTICLE);
  setItemData(col++, tr("Multiple searches separated by commas"),
              Qt::ToolTipRole);

  insertItem(col, icon, tr("Keywords"), CDVFilter::CDV_KEYWORDS);
  setItemData(col++, tr("Search for CD/Vinyl with Keyword"), Qt::ToolTipRole);

  insertItem(col, icon, tr("Author"), CDVFilter::CDV_AUTHOR);
  setItemData(col++, tr("Search for Prints and Stitches authors"),
              Qt::ToolTipRole);

  insertItem(col, icon, "ISBN/EAN", CDVFilter::CDV_ISBNEAN);
  setItemData(col++, tr("Search for ISBN/EAN"), Qt::ToolTipRole);
}

const QJsonObject CDVFilterSelect::getFilter(int index) {
  QJsonObject obj;
  switch (static_cast<CDVFilter>(itemData(index, Qt::UserRole).toInt())) {
  case CDV_TITLE: /**< @brief Titelsuche */
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("cv_title"));
    break;

  case CDV_KEYWORDS: /**< @brief Schlüsselwortsuche */
    obj.insert("search", QJsonValue("keywords"));
    obj.insert("fields", QJsonValue("cv_title,cv_keyword"));
    break;

  case CDV_AUTHOR: /**< @brief Autorensuche */
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("cv_author"));
    break;

  case CDV_ARTICLE: /**< @brief Artikel Nummernsuche */
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("cv_id"));
    break;

  case CDV_ISBNEAN: /**< @brief ISBN/EAN */
    obj.insert("search", QJsonValue("isbn"));
    obj.insert("fields", QJsonValue("cv_isbn"));
    break;

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("cv_title"));
    break;
  };

  return obj;
}
