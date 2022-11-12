// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetablemodel.h"

#include <ASettings>

PurchaseTableModel::PurchaseTableModel(AntiquaCRM::ASqlCore *sqlcore,
                                       QObject *parent)
    : QSqlTableModel{parent, sqlcore->db()}, p_cfg(parent), m_sql{sqlcore} {
  setObjectName("purchase_table_model");
  setTable("article_orders");
  setSort(0, Qt::AscendingOrder);
}

const QMap<int, QString> PurchaseTableModel::headerList() const {
  QMap<int, QString> m;
  m.insert(0, tr("Payment Id"));
  m.insert(1, tr("Order Id"));
  m.insert(2, tr("Article Id"));
  m.insert(3, tr("Customer Id"));
  m.insert(4, tr("Count"));
  m.insert(5, tr("Title"));
  m.insert(6, tr("Price"));
  m.insert(7, tr("Sell Price"));
  m.insert(8, tr("Modified"));
  m.insert(9, tr("Provider Id"));
  m.insert(10, tr("Type"));
  return m;
}

void PurchaseTableModel::reload() {
  if (queryHistory.isEmpty())
    return;

  setQuery(m_sql->query(queryHistory));
}

void PurchaseTableModel::setQueryId(const QString &field, qint64 id) {
  if (id < 1 || field.length() < 2)
    return;

  QString sql("SELECT * FROM " + tableName() + " WHERE ");
  sql.append(field + "=" + QString::number(id));
  sql.append(" ORDER BY a_payment_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    queryHistory = sql;
    setQuery(q);
  }
}

bool PurchaseTableModel::insertRow(qint64 orderId,
                                   const AntiquaCRM::OrderArticleItems &items) {
  QSqlRecord rec = record();
  QStringList fields;
  QStringList insert;
  QListIterator<AntiquaCRM::ArticleOrderItem> it(items);

  while (it.hasNext()) {
    AntiquaCRM::ArticleOrderItem article = it.next();
    QSqlField f = rec.field(article.key);
    if (!f.isValid())
      continue;

    if (f.requiredStatus() == QSqlField::Required && article.value.isNull()) {
      qWarning("Fatal: Missing value %s.", qPrintable(f.name()));
      return false;
    }

    if(f.name() == "a_payment_id")
      continue;

    fields << article.key;
    if (f.type() == QVariant::DateTime) {
      insert << "'" + article.value.toDateTime().toString() + "'";
    } else if (f.type() == QVariant::Int) {
      insert << QString::number(article.value.toInt());
    } else if (f.type() == QVariant::Double) {
      insert << QString::number(article.value.toDouble());
    } else {
      insert << "'" + article.value.toString() + "'";
    }
  }

  QString sql("INSERT INTO " + tableName() + " (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(insert.join(","));
  sql.append(") RETURNING a_payment_id;");

  qDebug() << orderId << sql;

  return false;
}

const QList<int> PurchaseTableModel::editableColumns() {
  QList<int> l;
  l.append(4);
  l.append(6);
  l.append(7);
  l.append(10);
  return l;
}

bool PurchaseTableModel::removeRows(int row, int count,
                                    const QModelIndex &parent) {
  Q_UNUSED(count);
  if (!parent.isValid())
    return false;

  QSqlRecord rec = record(row);
  if (rec.count() < 1)
    return false;

  qint64 a_payment_id = rec.field(0).value().toInt();
  if (a_payment_id < 1)
    return false;

  QString sql("DELETE FROM " + tableName() + " WHERE a_payment_id=");
  sql.append(QString::number(a_payment_id) + ";");
  m_sql->query(sql);
  return (m_sql->lastError().isEmpty());
}

QVariant PurchaseTableModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const {
  if (orientation != Qt::Horizontal)
    return QSqlTableModel::headerData(section, orientation, role);

  if (role == Qt::DecorationRole && editableColumns().contains(section)) {
    return QIcon("://icons/edit.png");
  }

  if (role != Qt::DisplayRole)
    return QSqlTableModel::headerData(section, orientation, role);

  QMap<int, QString> map = headerList();
  if (section > map.size())
    return tr("Unknown");

  return map.value(section) + " ";
}

QVariant PurchaseTableModel::data(const QModelIndex &item, int role) const {
  if (!item.isValid())
    return QVariant();

  QVariant value = QSqlTableModel::data(item, role);
  if (role != Qt::DisplayRole || !editableColumns().contains(item.column()))
    return value;

  if (item.column() == 6 || item.column() == 7) {
    QString currency = p_cfg.value("payment/currency", "€").toString();
    double d = value.toDouble();
    return QString().setNum(d, 'f', 2) + " " + currency;
  }

  if (item.column() == 10) {
    switch (static_cast<AntiquaCRM::ArticleType>(value.toInt())) {
    case (AntiquaCRM::ArticleType::BOOK):
      return tr("Book");

    case (AntiquaCRM::ArticleType::MEDIA):
      return tr("Media");

    case (AntiquaCRM::ArticleType::PRINTS):
      return tr("Graphic");

    case (AntiquaCRM::ArticleType::OTHER):
      return tr("Other");

    default:
      return tr("Not set");
    };
  }

  return value;
}
