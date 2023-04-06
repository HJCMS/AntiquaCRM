// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksselectfilter.h"

#include <AntiquaInput>

BooksSelectFilter::BooksSelectFilter(QWidget *parent) : QComboBox{parent} {
  setWhatsThis(
      tr("With this button you can optimize your search with some filters."));
  setSizeAdjustPolicy(QComboBox::AdjustToContents);

  QIcon _icon = AntiquaCRM::AntiquaApplIcon("view-search");
  insertItem(0, _icon, tr("Book Title and Author"), BOOK_TITLE_AUTHOR);
  setItemData(0, tr("Search in Book titles and authors"), Qt::ToolTipRole);

  insertItem(1, _icon, tr("Book Title and Keyword"), BOOK_TITLE_KEYWORD);
  setItemData(1, tr("Search in Book titles"), Qt::ToolTipRole);

  insertItem(2, _icon, tr("Article Id"), BOOK_ARTICLE_ID);
  setItemData(2, tr("Multiple searches separated by commas"), Qt::ToolTipRole);

  insertItem(3, _icon, tr("ISBN"), BOOK_ISBN);
  setItemData(3, tr("Only Numbers are allowed for input"), Qt::ToolTipRole);

  insertItem(4, _icon, tr("Author"), BOOK_AUTHORS);
  setItemData(4, tr("Search for Book authors"), Qt::ToolTipRole);

  insertItem(5, _icon, tr("Book Publisher"), BOOK_PUBLISHER);
  setItemData(5, tr("Search Book Publishers"), Qt::ToolTipRole);

  insertItem(6, _icon, tr("Duration by Keyword"), BOOK_STORAGE);
  setItemData(6, tr("Search by keyword and storage location"), Qt::ToolTipRole);
}

BooksSelectFilter::Filter BooksSelectFilter::currentFilter(int index) {
  qint8 _i = (index >= 0) ? index : currentIndex();
  qint8 _f = itemData(_i, Qt::UserRole).toInt();
  return static_cast<BooksSelectFilter::Filter>(_f);
}

const QJsonObject BooksSelectFilter::getFilter(int index) {
  QJsonObject obj;
  switch (currentFilter(index)) {
  case (BOOK_TITLE_AUTHOR): {
    obj.insert("search", QJsonValue("title_and_author"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_author"));
    break;
  }

  case (BOOK_TITLE_KEYWORD): {
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_keyword"));
    break;
  }

  case (BOOK_ARTICLE_ID): {
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("ib_id"));
    break;
  }

  case (BOOK_ISBN): {
    obj.insert("search", QJsonValue("isbn"));
    obj.insert("fields", QJsonValue("ib_isbn"));
    break;
  }

  case (BOOK_AUTHORS): {
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("ib_author"));
    break;
  }

  case (BOOK_PUBLISHER): {
    obj.insert("search", QJsonValue("publisher"));
    obj.insert("fields", QJsonValue("ib_publisher"));
    break;
  }

  case (BOOK_STORAGE): {
    obj.insert("search", QJsonValue("storage"));
    obj.insert("fields", QJsonValue("ib_storage"));
    break;
  }

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended"));
    break;
  };

  //#ifdef ANTIQUA_DEVELOPEMENT
  //  qDebug() << Q_FUNC_INFO << obj;
  //#endif

  return obj;
}
