// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetablemodel.h"

PurchaseTableModel::PurchaseTableModel(QObject *parent)
    : QAbstractTableModel{parent} {
  AntiquaCRM::ASettings cfg(this);
  currency = cfg.value("payment/currency", "€").toString();
}

const QString PurchaseTableModel::articleType(int type) {
  switch (static_cast<AntiquaCRM::ArticleType>(type)) {
  case AntiquaCRM::ArticleType::BOOK:
    return tr("Book");

  case AntiquaCRM::ArticleType::MEDIA:
    return tr("Media");

  case AntiquaCRM::ArticleType::PRINTS:
    return tr("Print");

  case AntiquaCRM::ArticleType::OTHER:
    return tr("Other");

  default:
    return tr("Unknown");
  }
}

const AntiquaCRM::ATableHeaderColumn
PurchaseTableModel::headerColumn(int column) {
  AntiquaCRM::ATableHeaderColumn col;
  switch (column) {
  case 0:
    col = AntiquaCRM::ATableHeaderColumn("a_payment_id", tr("Payment Id"));
    break;

  case 1:
    col = AntiquaCRM::ATableHeaderColumn("a_order_id", tr("Order Id"));
    break;

  case 2:
    col = AntiquaCRM::ATableHeaderColumn("a_article_id", tr("Article Id"));
    break;

  case 3:
    col = AntiquaCRM::ATableHeaderColumn("a_customer_id", tr("Customer Id"));
    break;

  case 4:
    col = AntiquaCRM::ATableHeaderColumn("a_type", tr("Type"));
    break;

  case 5:
    col = AntiquaCRM::ATableHeaderColumn("a_count", tr("Count"));
    break;

  case 6:
    col = AntiquaCRM::ATableHeaderColumn("a_price", tr("Price"));
    break;

  case 7:
    col = AntiquaCRM::ATableHeaderColumn("a_sell_price", tr("Sell Price"));
    break;

  case 8:
    col = AntiquaCRM::ATableHeaderColumn("a_title", tr("Title"));
    break;

  case 9:
    col = AntiquaCRM::ATableHeaderColumn("a_provider_id", tr("Provider Id"));
    break;

  case 10:
    col = AntiquaCRM::ATableHeaderColumn("a_modified", tr("Modified"));
    break;

  default:
    break;
  };
  return col;
}

const QList<int> PurchaseTableModel::editableColumns() {
  QList<int> l;
  l.append(4); /**< a_type */
  l.append(7); /**< a_sell_price */
  return l;
}

bool PurchaseTableModel::addArticle(const AntiquaCRM::OrderArticleItems &item) {
  beginResetModel();
  int pastCount = rowCount();
  articleRows.insert(rowCount(), item);
  insertRows(rowCount(), 1);
  endResetModel();
  return (rowCount() > pastCount);
}

void PurchaseTableModel::clear() {
  beginResetModel();
  articleRows.clear();
  endResetModel();
}

bool PurchaseTableModel::removeRows(int row, int count,
                                    const QModelIndex &parent) {
  if (parent.isValid() || row < 0 || count <= 0)
    return false;
  else if (row + count > rowCount())
    return false;
  else if (!count)
    return true;

  QList<AntiquaCRM::OrderArticleItems> articles;
  QMapIterator<int, AntiquaCRM::OrderArticleItems> it(articleRows);
  while (it.hasNext()) {
    it.next();
    if (it.key() != row) {
      articles.append(it.value());
    }
  }
  return addArticles(articles);
}

bool PurchaseTableModel::addArticles(
    const QList<AntiquaCRM::OrderArticleItems> &items) {
  if (items.size() < 1)
    return false;

  int row = 0;

  clear();

  QListIterator<AntiquaCRM::OrderArticleItems> articles(items);
  beginInsertRows(createIndex(row, 0), 0, items.size());
  while (articles.hasNext()) {
    articleRows.insert(row, articles.next());
    row++;
  }
  endInsertRows();
  return (articleRows.size() > 0);
}

int PurchaseTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return articleRows.size();
}

int PurchaseTableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return table_columns;
}

QVariant PurchaseTableModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const {
  if (orientation != Qt::Horizontal) {
    if (role == Qt::DisplayRole)
      return section + 1;

    return QVariant();
  }

  if (orientation == Qt::Horizontal && role == Qt::DecorationRole) {
    if (editableColumns().contains(section))
      return QIcon("://icons/edit.png");

    return QVariant();
  }

  AntiquaCRM::ATableHeaderColumn info = headerColumn(section);
  if (role == Qt::DisplayRole)
    return info.name() + " ";

  if (role == Qt::EditRole)
    return info.field();

  return QVariant();
}

bool PurchaseTableModel::setData(const QModelIndex &index,
                                 const QVariant &value, int role) {
  Q_UNUSED(role);
  if (!index.isValid())
    return false;

  AntiquaCRM::OrderArticleItems list = articleRows.value(index.row());
  if (list.size() < 1)
    return false;

  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  AntiquaCRM::OrderArticleItems row;
  for (int c = 0; c < list.size(); ++c) {
    AntiquaCRM::ArticleOrderItem col;
    col.key = list.at(c).key;
    if (list.at(c).key == info.field()) {
      col.value = value;
    } else {
      col.value = list.at(c).value;
    }
    row.append(col);
  }
  articleRows[index.row()] = row;

  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(articleRows.size(), table_columns);
  emit dataChanged(topLeft, bottomRight);

  return true;
}

const QString PurchaseTableModel::field(const QModelIndex &index) const {
  if (!index.isValid())
    return QString();

  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  return info.field();
}

int PurchaseTableModel::columnIndex(const QString &fieldName) const {
  for (int c = 0; c < columnCount(); c++) {
    AntiquaCRM::ATableHeaderColumn info = headerColumn(c);
    if (info.field() == fieldName)
      return c;
  }
  return -1;
}

QVariant PurchaseTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return QVariant();

  QVariant buffer;
  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  AntiquaCRM::OrderArticleItems list = articleRows.value(index.row());
  for (int c = 0; c < list.size(); c++) {
    if (list.at(c).key == info.field()) {
      buffer = list.at(c).value;
      break;
    }
  }

  QMetaType::Type _type = info.type();
  if (_type == QMetaType::UnknownType) {
    if (buffer.type() == QVariant::DateTime)
      _type = QMetaType::QDateTime;
    else if (buffer.type() == QVariant::Int)
      _type = QMetaType::Long;
    else
      _type = QMetaType::QString;
  }

  if (role == Qt::EditRole) {
    switch (_type) {
    case QMetaType::Bool:
      return buffer.toBool();

    case QMetaType::Int:
    case QMetaType::Long:
    case QMetaType::ULong:
      return buffer.toInt();

    case QMetaType::LongLong:
    case QMetaType::ULongLong:
      return buffer.toLongLong();

    case QMetaType::Double:
      return buffer.toDouble();

    case QMetaType::QDateTime:
      return buffer.toDateTime();

    default: {
      // FIXME Max. Zeichenlänge beim Titel ist 80!
      QString str = buffer.toString();
      if (str.length() > max_string_length) {
        int l = (max_string_length - 4);
        return str.left(l) + "...";
      }
      return str;
    }
    };
  }

  if (role == Qt::DisplayRole) {
    switch (_type) {
    case QMetaType::Bool:
      return buffer.toBool() ? tr("Yes") : tr("No");

    case QMetaType::Int:
      if (info.field() == "a_type") {
        return articleType(buffer.toInt());
      } else {
        return buffer;
      }

    case QMetaType::Double: {
      if (info.field().contains("_price")) {
        double p = buffer.toDouble();
        QString str = QString::number(p, 'f', 2);
        str.append(" " + currency);
        return str;
      }
    }

    case QMetaType::QDateTime: {
      QDateTime dt = buffer.toDateTime();
      return dt.toString(ANTIQUACRM_SHORT_DATE_DISPLAY);
    }

    default:
      return buffer.toString();
    };
  }

  return buffer;
}

Qt::ItemFlags PurchaseTableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  if (editableColumns().contains(index.column())) {
    return flags | Qt::ItemIsEditable;
  }

  return flags;
}
