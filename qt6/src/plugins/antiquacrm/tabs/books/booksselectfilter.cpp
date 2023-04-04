// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksselectfilter.h"

#include <AntiquaInput>

BooksSelectFilter::BooksSelectFilter(QWidget *parent) : QComboBox{parent} {
  setWhatsThis(
      tr("With this button you can optimize your search with some filters."));
  setSizeAdjustPolicy(QComboBox::AdjustToContents);

  QIcon _icon = AntiquaCRM::AntiquaApplIcon("view-search");
  insertItem(0, _icon, tr("Book Title and Author"), Filter::BooksAuthor);
  setItemData(0, tr("Search in Book titles and authors"), Qt::ToolTipRole);

  insertItem(1, _icon, tr("Book Title and Keyword"), Filter::BooksKeyword);
  setItemData(1, tr("Search in Book titles"), Qt::ToolTipRole);

  insertItem(2, _icon, tr("Article Id"), Filter::ArticleId);
  setItemData(2, tr("Multiple searches separated by commas"), Qt::ToolTipRole);

  insertItem(3, _icon, tr("ISBN"), Filter::ISBN);
  setItemData(3, tr("Only Numbers are allowed for input"), Qt::ToolTipRole);

  insertItem(4, _icon, tr("Author"), Filter::Authors);
  setItemData(4, tr("Search for Book authors"), Qt::ToolTipRole);

  insertItem(5, _icon, tr("Book Publisher"), Filter::Publisher);
  setItemData(5, tr("Search Book Publishers"), Qt::ToolTipRole);

  insertItem(6, _icon, tr("Duration by Keyword"), Filter::Storage);
  setItemData(6, tr("Search by keyword and storage location"), Qt::ToolTipRole);
}

const QJsonObject BooksSelectFilter::getFilter(int index) {
  QVariant val = itemData(index, Qt::UserRole);
  BooksSelectFilter::Filter f = qvariant_cast<BooksSelectFilter::Filter>(val);
  QJsonObject obj;
  switch (f) {
  case (Filter::BooksAuthor): {
    obj.insert("search", QJsonValue("title_and_author"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_author"));
    break;
  }

  case (Filter::BooksKeyword): {
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_keyword"));
    break;
  }

  case (Filter::ArticleId): {
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("ib_id"));
    break;
  }

  case (Filter::ISBN): {
    obj.insert("search", QJsonValue("isbn"));
    obj.insert("fields", QJsonValue("ib_isbn"));
    break;
  }

  case (Filter::Authors): {
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("ib_author"));
    break;
  }

  case (Filter::Publisher): {
    obj.insert("search", QJsonValue("publisher"));
    obj.insert("fields", QJsonValue("ib_publisher"));
    break;
  }

  case (Filter::Storage): {
    obj.insert("search", QJsonValue("storage"));
    obj.insert("fields", QJsonValue("ib_storage"));
    break;
  }

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended"));
    break;
  };

  return obj;
}
