// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksselectfilter.h"

BooksSelectFilter::BooksSelectFilter(QWidget *parent)
    : AntiquaCRM::TabsSearchBarFilter{parent} {
  setObjectName("books_filter_selecter");
  createItemData();
}

void BooksSelectFilter::createItemData() {
  int _i = 0;
  const QString _prefix(tr("Searches books") + " ");
  insertItem(_i, defaultIcon(), tr("Title and Keyword"),
             AntiquaCRM::SearchBarFilter::SBF_TITLES_KEYWORDS);
  setItemData(_i++, _prefix + tr("by title with keywords."), Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Title and Authors"),
             AntiquaCRM::SearchBarFilter::SBF_TITLES_AUTHORS);
  setItemData(_i++, _prefix + tr("by title with authors."), Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Article Id"),
             AntiquaCRM::SearchBarFilter::SBF_ARTICLE_IDS);
  setItemData(_i++, tr("Multiple Article numbers separated by commas!"),
              Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("ISBN"),
             AntiquaCRM::SearchBarFilter::SBF_ISBN_GTIN);
  setItemData(_i++, tr("Only Numbers are allowed here!"), Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Authors"),
             AntiquaCRM::SearchBarFilter::SBF_AUTHORS_ARTISTS);
  setItemData(_i++, _prefix + tr("for authors."), Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Publisher"),
             AntiquaCRM::SearchBarFilter::SBF_PUBLISHERS);
  setItemData(_i++, _prefix + tr("for publishers."), Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Storage"),
             AntiquaCRM::SearchBarFilter::SBF_STORAGES);
  setItemData(_i++, tr("by storage location."), Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Keywords"),
             AntiquaCRM::SearchBarFilter::SBF_KEYWORDS);
  setItemData(_i++, tr("by keywords."), Qt::ToolTipRole);
}

const QJsonObject BooksSelectFilter::getFilter(int index) {
  QJsonObject obj;
  switch (getValue(index)) {
  case (AntiquaCRM::SearchBarFilter::SBF_TITLES_AUTHORS): {
    obj.insert("search", QJsonValue("title_and_author"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_author"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_TITLES_KEYWORDS): {
    obj.insert("search", QJsonValue("title_and_keywords"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_keyword"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_ARTICLE_IDS): {
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("ib_id"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_ISBN_GTIN): {
    obj.insert("search", QJsonValue("isbn"));
    obj.insert("fields", QJsonValue("ib_isbn"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_AUTHORS_ARTISTS): {
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("ib_author"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_PUBLISHERS): {
    obj.insert("search", QJsonValue("publisher"));
    obj.insert("fields", QJsonValue("ib_publisher"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_STORAGES): {
    obj.insert("search", QJsonValue("storage"));
    obj.insert("fields", QJsonValue("__CUSTOM__"));
    break;
  }

  case (AntiquaCRM::SearchBarFilter::SBF_KEYWORDS): {
    obj.insert("search", QJsonValue("keywords"));
    obj.insert("fields", QJsonValue("__CUSTOM__"));
    break;
  }

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended"));
    break;
  };

  // qDebug() << Q_FUNC_INFO << obj;
  return obj;
}
