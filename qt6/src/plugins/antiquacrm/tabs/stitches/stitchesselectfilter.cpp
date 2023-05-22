// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitchesselectfilter.h"

#include <AntiquaInput>

StitchesSelectFilter::StitchesSelectFilter(QWidget *parent)
    : QComboBox{parent} {
  setWhatsThis(
      tr("With this button you can optimize your search with some filters."));
  setSizeAdjustPolicy(QComboBox::AdjustToContents);

  QString _prefix = tr("Search Book with") + " ";
  QIcon _icon = AntiquaCRM::AntiquaApplIcon("view-search");
  insertItem(0, _icon, tr("Title and Author"), STITCHES_TITLE_AUTHOR);
  setItemData(0, _prefix + tr("Title and Author"), Qt::ToolTipRole);

  insertItem(1, _icon, tr("Title and Keyword"), STITCHES_TITLE_KEYWORD);
  setItemData(1, _prefix + tr("Title and Keyword"), Qt::ToolTipRole);

  insertItem(2, _icon, tr("Article Id"), STITCHES_ARTICLE_ID);
  setItemData(2, tr("Multiple searches separated by commas!"), Qt::ToolTipRole);

  insertItem(3, _icon, tr("Author"), STITCHES_AUTHORS);
  setItemData(3, _prefix + tr("Author"), Qt::ToolTipRole);

  insertItem(4, _icon, tr("Publisher"), STITCHES_PUBLISHER);
  setItemData(4, _prefix + tr("Publisher"), Qt::ToolTipRole);

  insertItem(5, _icon, tr("Storage"), STITCHES_STORAGE);
  setItemData(5, tr("In Storage location by Keyword"), Qt::ToolTipRole);
}

StitchesSelectFilter::Filter StitchesSelectFilter::currentFilter(int index) {
  qint8 _i = (index >= 0) ? index : currentIndex();
  return qvariant_cast<StitchesSelectFilter::Filter>(
      itemData(_i, Qt::UserRole));
}

const QJsonObject StitchesSelectFilter::getFilter(int index) {
  QJsonObject obj;
  switch (currentFilter(index)) {
  case (STITCHES_TITLE_AUTHOR): {
    obj.insert("search", QJsonValue("title_and_author"));
    obj.insert("fields", QJsonValue("ip_title,ip_title_extended,ip_author"));
    break;
  }

  case (STITCHES_TITLE_KEYWORD): {
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ip_title,ip_title_extended,ip_keyword"));
    break;
  }

  case (STITCHES_ARTICLE_ID): {
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("ip_id"));
    break;
  }

  case (STITCHES_AUTHORS): {
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("ip_author"));
    break;
  }

  case (STITCHES_PUBLISHER): {
    obj.insert("search", QJsonValue("publisher"));
    obj.insert("fields", QJsonValue("ip_publisher"));
    break;
  }

  case (STITCHES_STORAGE): {
    obj.insert("search", QJsonValue("storage"));
    obj.insert("fields", QJsonValue("ip_storage"));
    break;
  }

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ip_title,ip_title_extended"));
    break;
  };

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << obj;
#endif

  return obj;
}
