// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableview.h"
#include "orderstabledelegate.h"
#include "orderstablemodel.h"

#include <AntiquaCRM>
#include <QAbstractItemView>
#include <QAction>
#include <QFont>
#include <QHeaderView>
#include <QIcon>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPalette>

OrdersTableView::OrdersTableView(QWidget *parent, bool readOnly)
    : QTableView{parent} {
  setObjectName("OrdersTableView");
  setWindowTitle("Purchases [*]");
  setToolTip(tr("Article purchases"));
  setSortingEnabled(false);
  setAlternatingRowColors(true);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  // Einfaches Auswählen kein Strg+Shift+Mausklick
  setSelectionMode(QAbstractItemView::SingleSelection);
  setProtected(readOnly);

  m_model = new OrdersTableModel(this);
  setModel(m_model);

  if (!readOnly) {
    // Editoren initialisieren!
    m_delegate = new OrdersTableDelegate(this);
    setItemDelegate(m_delegate);
  }

  QHeaderView *m_header = horizontalHeader();
  m_header->setHighlightSections(true);
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_header);

  connect(m_model,
          SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          SLOT(articleChanged(const QModelIndex &, const QModelIndex &)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(rowSelected(const QModelIndex &)));
}

void OrdersTableView::changeEvent(QEvent *event) {
  if (event->type() == QEvent::ModifiedChange && isWindowModified())
    parentWidget()->setWindowModified(true);

  // qDebug() << Q_FUNC_INFO << isWindowModified();
  QTableView::changeEvent(event);
}

void OrdersTableView::paintEvent(QPaintEvent *event) {
  if (rowCount() == 0) {
    QStringList _info;
    _info << tr("Please insert here, the required Order article.");
    _info << tr("An Order requires minimum one Article!");
    QPainter painter(viewport());
    painter.setBrush(palette().text());
    painter.setFont(font());
    painter.setOpacity(0.8);
    painter.drawText(rect(), Qt::AlignCenter, _info.join("\n"));
  }
  QTableView::paintEvent(event);
}

void OrdersTableView::contextMenuEvent(QContextMenuEvent *event) {
  if (editTriggers() == QAbstractItemView::NoEditTriggers)
    return;

  p_modelIndex = indexAt(event->pos());
  if (!p_modelIndex.isValid())
    return;

  QMenu *m = new QMenu("Actions", this);
  QAction *ac_del = m->addAction(AntiquaCRM::antiquaIcon("database-remove"),
                                 tr("Delete selected Article"));
  ac_del->setEnabled((m_model->rowCount() > 1));
  connect(ac_del, SIGNAL(triggered()), SLOT(addDeleteQuery()));

  QAction *ac_open = m->addAction(AntiquaCRM::antiquaIcon("view-info"),
                                  tr("Open article ..."));
  ac_open->setEnabled((m_model->rowCount() > 0));
  connect(ac_open, SIGNAL(triggered()), SLOT(addArticleQuery()));
  m->exec(event->globalPos());
  delete m;
}

void OrdersTableView::articleChanged(const QModelIndex &topLeft,
                                     const QModelIndex &bottomRight) {
  Q_UNUSED(topLeft);
  resizeColumnToContents(bottomRight.column());
  horizontalHeader()->setStretchLastSection(true);
  setWindowModified(true);
  // qDebug() << Q_FUNC_INFO;
}

void OrdersTableView::rowSelected(const QModelIndex &index) {
  QModelIndex _index = index.sibling(index.row(), 0);
  if (!_index.isValid())
    return;

  qint64 _id = m_model->data(_index, Qt::EditRole).toInt();
  if (_id > 0)
    emit sendPaymentId(_id);
}

void OrdersTableView::addDeleteQuery() {
  QModelIndex _index = p_modelIndex.sibling(p_modelIndex.row(), 0);
  if (!_index.isValid())
    return;

  qint64 _id = m_model->data(_index, Qt::EditRole).toInt();
  if (m_model->removeRow(_index.row())) {
    setWindowModified(true);
    if (_id > 0) { // SQL DELETE call
      QString _sql("DELETE FROM article_orders WHERE a_payment_id=");
      _sql.append(QString::number(_id) + ";");
      sql_cache << _sql;
    }
  }
}

void OrdersTableView::addArticleQuery() {
  QModelIndex _a_index = p_modelIndex.sibling(p_modelIndex.row(), 2);
  if (!_a_index.isValid())
    return;

  qint64 _aid = m_model->data(_a_index, Qt::EditRole).toInt();
  if (_aid < 1)
    return;

  const QString _strid = QString::number(_aid);
  AntiquaCRM::ArticleType _type;
  AntiquaCRM::ASqlCore psql(this);
  QSqlQuery _q = psql.query("SELECT func_get_article_type(" + _strid + ");");
  if (_q.size() > 0) {
    _q.next();
    _type = static_cast<AntiquaCRM::ArticleType>(_q.value(0).toInt());
  } else {
    _type = AntiquaCRM::ArticleType::BOOK;
  }

  QJsonObject _action;
  _action.insert("ACTION", "open_article");
  // mediaType
  switch (_type) {
  case AntiquaCRM::ArticleType::MEDIA: /**< Film & Tonträger */
    _action.insert("TARGET", "cdvinyl_tab");
    break;

  case AntiquaCRM::ArticleType::PRINTS: /**< Drucke & Stiche */
    _action.insert("TARGET", "printsstitches_tab");
    break;

  case AntiquaCRM::ArticleType::OTHER: /**< Various */
    _action.insert("TARGET", "various_tab");
    break;

  default: /**< Default: Books */
    _action.insert("TARGET", "books_tab");
    break;
  };
  _action.insert("VALUE", _aid);

  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(_action))
    m_sock->close();

  // qDebug() << Q_FUNC_INFO << _action;
}

void OrdersTableView::setProtected(bool readOnly) {
  if (readOnly) {
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
  } else {
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setEditTriggers(QAbstractItemView::DoubleClicked);
  }
}

void OrdersTableView::clearContents() {
  if (rowCount() > 0) {
    m_model->clearContents();
    setWindowModified(true);
  }
  sql_cache.clear();
}

void OrdersTableView::addArticle(const AntiquaCRM::OrderArticleItems &order) {
  if (m_model->addArticle(order)) {
    setWindowModified(true);
    return;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qWarning("-- OrdersTableView::addArticle -- Rejected --");
#endif
}

bool OrdersTableView::updateRefundCoast(double price) {
  if (rowCount() < 1)
    return false;

  int _count = 0;
  int _column = m_model->columnIndex("a_refunds_cost");
  for (int r = 0; r < rowCount(); r++) {
    if (m_model->setData(m_model->index(r, _column), price, Qt::EditRole))
      _count++;
  }
  return (_count > 0);
}

int OrdersTableView::rowCount() { return m_model->rowCount(); }

bool OrdersTableView::isEmpty() { return (m_model->rowCount() < 1); }

void OrdersTableView::hideColumns(const QStringList &list) {
  foreach (QString fieldName, list) {
    int column = m_model->columnIndex(fieldName);
    horizontalHeader()->setSectionHidden(column, true);
  }
}

bool OrdersTableView::addArticles(
    const QList<AntiquaCRM::OrderArticleItems> &items) {
  if (items.size() < 1)
    return false;

  // Note clear not need at this point!
  return m_model->addArticles(items);
}

bool OrdersTableView::setOrderId(qint64 orderId) {
  int _count = 0;
  int _index = m_model->columnIndex("a_order_id");
  for (int r = 0; r < m_model->rowCount(); r++) {
    if (m_model->setData(m_model->index(r, _index), orderId, Qt::EditRole))
      _count++;
  }
  return (_count > 0);
}

const QStringList OrdersTableView::getSqlQuery() {
  // Ausgabe
  QStringList queries;
  // Werden bei INSERT|UPDATE Ignoriert!
  QStringList ignoreList({"a_payment_id", "a_modified"});
  // Starte Tabellen abfrage ...
  for (int r = 0; r < m_model->rowCount(); r++) {
    // Suche "a_payment_id" für INSERT|UPDATE Prüfung!
    qint64 pid = m_model->data(m_model->index(r, 0), Qt::EditRole).toInt();
    QStringList update;  /**< SQL UPDATE SET */
    QStringList fields;  /**< SQL INSERT FIELDS */
    QStringList inserts; /**< SQL INSERT VALUES */
    for (int c = 0; c < m_model->columnCount(); c++) {
      QModelIndex _index = m_model->index(r, c);
      QString _fieldName = m_model->field(_index);
      if (ignoreList.contains(_fieldName))
        continue;

      QVariant _value = m_model->data(_index, Qt::EditRole);
      QMetaType _type = _value.metaType();
      // qDebug() << Q_FUNC_INFO << _fieldName << _value << _type.id();
      if (pid > 0) {
        if (_type.id() == QMetaType::QString)
          update << QString(_fieldName + "='" + _value.toString() + "'");
        else
          update << QString(_fieldName + "=" + _value.toString());
      } else {
        fields << _fieldName;
        if (_type.id() == QMetaType::QString)
          inserts << "'" + _value.toString() + "'";
        else
          inserts << _value.toString();
      }
    }

    if (pid > 0) {
      QString sql("UPDATE article_orders SET ");
      sql.append(update.join(","));
      sql.append(" WHERE a_payment_id=" + QString::number(pid) + ";");
      queries << sql;
      update.clear();
    } else {
      QString sql("INSERT INTO article_orders (");
      sql.append(fields.join(","));
      sql.append(") VALUES (");
      sql.append(inserts.join(","));
      sql.append(");");
      queries << sql;
      fields.clear();
      inserts.clear();
    }
  }

  if (!sql_cache.isEmpty())
    queries << sql_cache;

  return queries;
}

bool OrdersTableView::articleExists(qint64 articleId) {
  if (rowCount() < 1)
    return false;

  int _column = m_model->columnIndex("a_article_id");
  for (int r = 0; r < rowCount(); r++) {
    QModelIndex _index = m_model->index(r, _column);
    if (m_model->data(_index, Qt::EditRole).toInt() == articleId) {
      setCurrentIndex(_index);
      return true;
    }
  }
  return false;
}

qint64 OrdersTableView::getPaymentId(int row) {
  if (row >= 0) {
    int _column = m_model->columnIndex("a_payment_id");
    const QModelIndex _index(m_model->index(row, _column));
    return m_model->data(_index, Qt::EditRole).toInt();
  }
  return -1;
}

double OrdersTableView::getRefundingCost(int row) {
  if (row >= 0) {
    int _column = m_model->columnIndex("a_refunds_cost");
    const QModelIndex _index(m_model->index(row, _column));
    return m_model->data(_index, Qt::EditRole).toDouble();
  }
  return 0.00;
}

QMap<qint64, double> OrdersTableView::getRefundingCosts() {
  QMap<qint64, double> _m;
  if (rowCount() == 0)
    return _m;

  for (int r = 0; r < rowCount(); r++) {
    qint64 _id = getPaymentId(r);
    double _cost = getRefundingCost(r);
    if (_id > 0)
      _m.insert(_id, _cost);
  }
  return _m;
}

const QList<AntiquaCRM::OrderArticleItems> OrdersTableView::getRefundData() {
  QList<AntiquaCRM::OrderArticleItems> _refunds;
  QModelIndex _parent = m_model->parentIndex();
  QStringList _forceNull({"a_payment_id", "a_order_id"});
  for (int r = 0; r < rowCount(); ++r) {
    AntiquaCRM::OrderArticleItems _items;
    for (int c = 0; c < m_model->columnCount(); c++) {
      QModelIndex _index = _parent.sibling(r, c);
      AntiquaCRM::ArticleOrderItem _item;
      _item.key = m_model->field(_index);
      if (_forceNull.contains(_item.key))
        _item.value = 0;
      else
        _item.value = m_model->data(_index, Qt::EditRole);

      _items.append(_item);
    }
    if (_items.size() > 0)
      _refunds.append(_items);
  }
  return _refunds;
}
