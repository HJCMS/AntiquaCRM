// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "descwordstablemodel.h"
#include "antiquaicon.h"

DescWordsTableModel::DescWordsTableModel(QObject* parent)
    : AntiquaCRM::ASqlQueryModel{tableName(), parent} {
  setObjectName("descwords_table_model");
}

const QString DescWordsTableModel::fieldList() {
  return QString("aes_id,aes_type,aes_lang,aes_title,aes_data");
}

const QString DescWordsTableModel::tableName() {
  return QString("article_descriptions");
}

const QString DescWordsTableModel::orderColumn() {
  return QString("aes_id");
}

const QString DescWordsTableModel::sectionTitle(int type) {
  switch (static_cast<AntiquaCRM::ArticleType>(type)) {
    case AntiquaCRM::ArticleType::BOOK:
      return tr("Books");

    case AntiquaCRM::ArticleType::MEDIA:
      return tr("Multimedia");

    case AntiquaCRM::ArticleType::PRINTS:
      return tr("Prints & Stitches");

    case AntiquaCRM::ArticleType::OTHER:
      return tr("Other");

    default:
      return tr("Unknown");
  }
}

const QMap<int, QString> DescWordsTableModel::headerList() const {
  int i = 0;
  QMap<int, QString> map;
  map.insert(i++, tr("Id"));               // aes_id
  map.insert(i++, tr("Section"));          // aes_type
  map.insert(i++, tr("Language"));         // aes_lang
  map.insert(i++, tr("Menu title"));       // aes_title
  map.insert(i++, tr("Description data")); // aes_data
  map.insert(i++, tr("Unknown"));
  return map;
}

QVariant DescWordsTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    return verticalHeader(section, role);

  if (orientation == Qt::Horizontal && role == Qt::DecorationRole)
    return QVariant();

  if (role == Qt::EditRole) {
    return record().field(section).name();
  } else if (role != Qt::DisplayRole) {
    return QSqlQueryModel::headerData(section, orientation, role);
  }

  QMap<int, QString> map = headerList();
  if (section > map.size())
    return tr("Unknown");

  return setHeaderTitle(map.value(section));
}

QVariant DescWordsTableModel::data(const QModelIndex& item, int role) const {
  if (!item.isValid())
    return QVariant();

  if (item.column() == 1 && role == Qt::DisplayRole) {
    int _section = AntiquaCRM::ASqlQueryModel::data(item, Qt::EditRole).toInt();
    return sectionTitle(_section);
  } else if (item.column() == 2) {
    QString _lang = AntiquaCRM::ASqlQueryModel::data(item, Qt::EditRole).toString();
    if (role == Qt::DisplayRole) {
      if (_lang.contains("de_")) {
        return tr("German");
      } else {
        return tr("European");
      }
    } else if (role == Qt::DecorationRole) {
      if (_lang.contains("de_")) {
        return AntiquaCRM::antiquaIcon("de_DE");
      } else {
        return AntiquaCRM::antiquaIcon("european-flag");
      }
    }
  }

  return AntiquaCRM::ASqlQueryModel::data(item, role);
}
