// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstable.h"
#include "applsettings.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QSqlQuery>

ViewsTableModel::ViewsTableModel(const QSqlDatabase &db, QWidget *parent)
    : QSqlTableModel{parent, db},
      currency(ApplSettings().value("payment/currency").toByteArray()) {
  setObjectName("views_table_model");
}

const QHash<QString, QString> ViewsTableModel::translations() {
  QHash<QString, QString> l;
  l.insert("sl_identifier", tr("Identity"));
  l.insert("title", tr("Title"));
  l.insert("categorie", tr("Categorie"));
  l.insert("ib_title", tr("Title"));
  l.insert("ip_title", tr("Title"));
  l.insert("ib_price", tr("Price"));
  l.insert("price", tr("Price"));
  l.insert("counts", tr("Count"));
  l.insert("total_price", tr("total price"));
  l.insert("ib_author", tr("Author"));
  l.insert("sl_storage", tr("Storage"));
  l.insert("storages", tr("Storage"));
  l.insert("twofold_keywords", tr("Double Keywords"));
  l.insert("ib_id", tr("Article Id"));
  l.insert("ip_id", tr("Article Id"));
  l.insert("sl_id", tr("Archive Id"));
  // l.insert("",tr(""));
  return l;
}

QVariant ViewsTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();

  QString key = record().field(section).name();
  QHash<QString, QString> list = translations();
  return (list.contains(key)) ? " " + list.value(key) + " " : key;
}

QVariant ViewsTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole &&
                           role == Qt::DecorationRole))
    return val;

  QVariant item = QSqlTableModel::data(index, role);
  if (item.type() == QVariant::Double) {
    QString price = QString::number(item.toDouble(), '0', 2);
    price.append(" ");
    price.append(currency);
    return price;
  }

  return item;
}

ViewsTable::ViewsTable(QWidget *parent) : QTableView{parent} {
  setObjectName("views_table");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(true);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::MultiSelection);

  m_sql = new HJCMS::SqlCore(this);

  m_model = new ViewsTableModel(m_sql->db(), this);
  m_model->setObjectName("views_table_model");
  setModel(m_model);

  /* Kopfzeilen anpassen */
  m_headerView = horizontalHeader();
  m_headerView->setDefaultAlignment(Qt::AlignCenter);
  m_headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_headerView->setStretchLastSection(true);
  setHorizontalHeader(m_headerView);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(articleClicked(const QModelIndex &)));
}

void ViewsTable::articleClicked(const QModelIndex &index) {
  QModelIndex mid = index.sibling(index.row(), 0);
  if (mid.isValid()) {
    int id = m_model->data(mid, Qt::DisplayRole).toInt();
    if (id > 0 && m_model->tableName().contains("_book_"))
      emit s_articleSelected(id, "bookeditor");
    else if (id > 0 && m_model->tableName().contains("_print_"))
      emit s_articleSelected(id, "printseditor");
    else if (id > 0 && m_model->tableName().contains("_customer_"))
      emit s_articleSelected(id, "customereditor");
    else
      qWarning("Unused Id: %s in Views Table.",
               qPrintable(QString::number(id)));
  }
}

void ViewsTable::sqlQuery(const QString &viewtable) {
  if (viewtable.isEmpty() || !viewtable.contains("view_"))
    return;

  m_model->setTable(viewtable);
  if (m_model->select()) {
    emit s_queryFinished();
  }
}

ViewsTable::~ViewsTable() {
  m_sql->deleteLater();
  m_model->deleteLater();
}
