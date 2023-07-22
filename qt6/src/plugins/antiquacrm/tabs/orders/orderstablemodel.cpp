// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstablemodel.h"

#include <AntiquaInput>
#include <QMetaType>

#ifdef ANTIQUA_DEVELOPEMENT
void __debug_article_items(AntiquaCRM::OrderArticleItems items) {
  qDebug() << "ArticleOrderItem" << items.size();
  QListIterator<AntiquaCRM::ArticleOrderItem> it(items);
  while (it.hasNext()) {
    AntiquaCRM::ArticleOrderItem sub = it.next();
    qDebug() << "- column:" << sub.key << sub.value;
  }
}
#endif

OrdersTableModel::OrdersTableModel(QObject *parent)
    : QAbstractTableModel{parent} {
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup("payment");
  currency = cfg.value("currency", "€").toString();
  vat_normal = cfg.value("vat1", 19).toInt();
  vat_reduced = cfg.value("vat2", 7).toInt();
  cfg.endGroup();
}

const QString OrdersTableModel::displayPrice(double price) const {
  QString str = QString::number(price, 'f', 2);
  str.append(" " + currency);
  return str.trimmed();
}

void OrdersTableModel::clearContents() {
  beginResetModel();
  articles.clear();
  endResetModel();
}

int OrdersTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return articles.size();
}

int OrdersTableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return p_columns;
}

QVariant OrdersTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (orientation == Qt::Vertical) {
    if (role == Qt::DisplayRole)
      return section + 1;

    return QVariant();
  }

  if (orientation == Qt::Horizontal && role == Qt::DecorationRole) {
    if (editableColumns().contains(section))
      return AntiquaCRM::antiquaIcon("document-edit");

    return QVariant();
  }

  AntiquaCRM::ATableHeaderColumn info = headerColumn(section);
  if (role == Qt::DisplayRole)
    return info.name() + " ";

  if (role == Qt::EditRole)
    return info.field();

  return QVariant();
}

QVariant OrdersTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return QVariant();

  QVariant buffer;
  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  AntiquaCRM::OrderArticleItems list = articles.value(index.row());
  for (int c = 0; c < list.size(); c++) {
    if (list.at(c).key == info.field()) {
      buffer = list.at(c).value;
      break;
    }
  }

  const QMetaType _type = buffer.metaType();
  if (_type.id() == QMetaType::UnknownType) {
    qWarning("Invalid MetaType:'%s'.", qPrintable(info.field()));
    return QVariant();
  }

  // BEGIN::EditRole
  if (role == Qt::EditRole) {
    switch (_type.id()) {
    case QMetaType::Bool:
      return buffer.toBool();

    case QMetaType::Int:
    case QMetaType::Long:
    case QMetaType::ULong:
      return buffer.toInt();

    case QMetaType::LongLong:
    case QMetaType::ULongLong:
      return buffer.toLongLong();

    case QMetaType::Float:
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
  // END::EditRole

  // BEGIN::DisplayRole
  const QString _fname = info.field();

  if (_fname == "a_type") {
    return articleType(buffer.toInt());
  }

  if (_fname == "a_tax") {
    TaxType _t = static_cast<TaxType>(buffer.toInt());
    return QString::number(taxValue(_t)) + "%";
  }

  if (_fname.contains("_price") || _fname.contains("_cost")) {
    return displayPrice(buffer.toDouble());
  }

  switch (_type.id()) {
  case QMetaType::Bool:
    return buffer.toBool() ? tr("Yes") : tr("No");

  case QMetaType::Int:
  case QMetaType::Long:
  case QMetaType::ULong: {
    return buffer.toInt();
  }

  case QMetaType::LongLong:
  case QMetaType::ULongLong:
    return buffer.toLongLong();

  case QMetaType::Float:
  case QMetaType::Double: {
    return buffer.toDouble();
  }

  case QMetaType::QDate: {
    QDate dt = buffer.toDate();
    return dt.toString(ANTIQUACRM_SHORT_DATE_DISPLAY);
  }

  case QMetaType::QDateTime: {
    QDateTime dt = buffer.toDateTime();
    return dt.toString(ANTIQUACRM_SHORT_DATE_DISPLAY);
  }

  default:
    return buffer.toString();
  };
  // END::DisplayRole

  return QVariant();
}

bool OrdersTableModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return false;

  AntiquaCRM::OrderArticleItems _list = articles.value(index.row());
  if (_list.size() < 1)
    return false;

  AntiquaCRM::ATableHeaderColumn _info = headerColumn(index.column());
  AntiquaCRM::OrderArticleItems _row;
  for (int c = 0; c < _list.size(); ++c) {
    AntiquaCRM::ArticleOrderItem _col;
    _col.key = _list.at(c).key;
    if (_list.at(c).key == _info.field()) {
      _col.value = value;
    } else {
      _col.value = _list.at(c).value;
    }
    _row.append(_col);
  }
  articles[index.row()] = _row;
  const QModelIndex topLeft = createIndex(0, 0);
  const QModelIndex bottomRight = createIndex(articles.size(), p_columns);
  emit dataChanged(topLeft, bottomRight);
  return true;
}

Qt::ItemFlags OrdersTableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  if (editableColumns().contains(index.column())) {
    return flags | Qt::ItemIsEditable;
  }

  return flags;
}

bool OrdersTableModel::removeRows(int row, int count,
                                  const QModelIndex &parent) {
  if (parent.isValid() || row < 0 || count <= 0)
    return false;
  else if (row + count > rowCount())
    return false;
  else if (!count)
    return true;

  QList<AntiquaCRM::OrderArticleItems> _items;
  QMapIterator<int, AntiquaCRM::OrderArticleItems> it(articles);
  while (it.hasNext()) {
    it.next();
    if (it.key() != row) {
      _items.append(it.value());
    }
  }
  return addArticles(_items);
}

const QString OrdersTableModel::field(const QModelIndex &index) const {
  if (!index.isValid())
    return QString();

  AntiquaCRM::ATableHeaderColumn info = headerColumn(index.column());
  return info.field();
}

int OrdersTableModel::columnIndex(const QString &fieldName) const {
  for (int c = 0; c < columnCount(); c++) {
    AntiquaCRM::ATableHeaderColumn info = headerColumn(c);
    if (info.field() == fieldName)
      return c;
  }
  return -1;
}

const AntiquaCRM::ATableHeaderColumn
OrdersTableModel::headerColumn(int column) {
  AntiquaCRM::ATableHeaderColumn _col;
  switch (column) {
  case 0:
    _col = AntiquaCRM::ATableHeaderColumn("a_payment_id", tr("Payment Id"));
    break;

  case 1:
    _col = AntiquaCRM::ATableHeaderColumn("a_order_id", tr("Order Id"));
    break;

  case 2:
    _col = AntiquaCRM::ATableHeaderColumn("a_article_id", tr("Article Id"));
    break;

  case 3:
    _col = AntiquaCRM::ATableHeaderColumn("a_customer_id", tr("Customer Id"));
    break;

  case 4:
    _col = AntiquaCRM::ATableHeaderColumn("a_type", tr("Type"));
    break;

  case 5:
    _col = AntiquaCRM::ATableHeaderColumn("a_count", tr("Count"));
    break;

  case 6:
    _col = AntiquaCRM::ATableHeaderColumn("a_price", tr("Price"));
    break;

  case 7:
    _col = AntiquaCRM::ATableHeaderColumn("a_sell_price", tr("Sell Price"));
    break;

  case 8:
    _col = AntiquaCRM::ATableHeaderColumn("a_tax", tr("VAT"));
    break;

  case 9:
    _col = AntiquaCRM::ATableHeaderColumn("a_title", tr("Title"));
    break;

  case 10:
    _col = AntiquaCRM::ATableHeaderColumn("a_modified", tr("Modified"));
    break;

  case 11:
    _col = AntiquaCRM::ATableHeaderColumn("a_provider_id", tr("Provider Id"));
    break;

  case 12:
    _col = AntiquaCRM::ATableHeaderColumn("a_refunds_cost", tr("Refunding"));
    break;

  default:
    _col = AntiquaCRM::ATableHeaderColumn("unknown", tr("Unknown"));
    break;
  };
  return _col;
}

bool OrdersTableModel::addArticle(const AntiquaCRM::OrderArticleItems &item) {
  beginResetModel();
  int pastCount = rowCount();
  articles.insert(rowCount(), item);
  insertRows(rowCount(), 1);
  endResetModel();
  return (rowCount() > pastCount);
}

bool OrdersTableModel::addArticles(
    const QList<AntiquaCRM::OrderArticleItems> &items) {
  if (items.size() < 1)
    return false;

  int row = 0;

  clearContents();

  QListIterator<AntiquaCRM::OrderArticleItems> it(items);
  beginInsertRows(createIndex(row, 0), 0, items.size());
  while (it.hasNext()) {
    AntiquaCRM::OrderArticleItems items = it.next();
    if (p_columns < items.size()) {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << "INVALID Column Count!" << Qt::endl
               << "Current" << p_columns << "Size" << items.size();

      __debug_article_items(items);
#else
      qWarning("OrdersTableModel: Invalid Order Article column Count!");
#endif
      p_columns = items.size();
    }
    articles.insert(row, items);
    row++;
  }
  endInsertRows();
  return (articles.size() > 0);
}

qreal OrdersTableModel::taxValue(OrdersTableModel::TaxType type) const {
  if (type == OrdersTableModel::TaxType::REDUCED)
    return vat_reduced;
  else
    return vat_normal;
}

const QList<int> OrdersTableModel::editableColumns() const {
  QList<int> l;
  l.append(4); /**< a_type */
  l.append(5); /**< a_count */
  l.append(7); /**< a_sell_price */
  return l;
}

const QString OrdersTableModel::articleType(int type) {
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
