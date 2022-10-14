// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookselectfilter.h"

#include <QIcon>

BookSelectFilter::BookSelectFilter(QWidget *parent) : QComboBox{parent}
{
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(":icons/view_search.png");
  insertItem(0, icon, tr("Book Title and Author"), Filter::BooksAuthor);
  setItemData(0, tr("Search in Book titles and authors"), Qt::ToolTipRole);

  insertItem(1, icon, tr("Book Title and Keyword"), Filter::BooksKeyword);
  setItemData(1, tr("Search in Book titles"), Qt::ToolTipRole);

  insertItem(2, icon, tr("Article Id"), Filter::ArticleId);
  setItemData(2, tr("Multiple searches separated by commas"), Qt::ToolTipRole);

  insertItem(3, icon, tr("ISBN"), Filter::ISBN);
  setItemData(3, tr("Only Numbers are allowed for input"), Qt::ToolTipRole);

  insertItem(4, icon, tr("Author"), Filter::Authors);
  setItemData(4, tr("Search for Book authors"), Qt::ToolTipRole);

  insertItem(5, icon, tr("Book Publisher"), Filter::Publisher);
  setItemData(5, tr("Search Book Publishers"), Qt::ToolTipRole);

  insertItem(6, icon, tr("Duration by Keyword"), Filter::Storage);
  setItemData(6, tr("Search by keyword and storage location"), Qt::ToolTipRole);
}

const QJsonObject BookSelectFilter::getFilter(int index) {
  int i = (index >= 0) ?: currentIndex();
  QVariant val = itemData(index, Qt::UserRole);
  BookSelectFilter::Filter f = qvariant_cast<BookSelectFilter::Filter>(val);
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
