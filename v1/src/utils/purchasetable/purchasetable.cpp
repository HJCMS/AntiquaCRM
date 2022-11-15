// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetable.h"
#include "purchasetableheader.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QTableWidgetItem>

PurchaseTable::PurchaseTable(QWidget *parent, bool readOnly)
    : QTableWidget{parent}, tableName{"article_orders"} {
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSortingEnabled(false);
  setSelectionMode(QAbstractItemView::SingleSelection);
  // setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  m_sql = new AntiquaCRM::ASqlCore(this);
  p_tableRecord = m_sql->record(tableName);

  AntiquaCRM::ASettings cfg(this);

  EditorProperties conf;
  conf.minPrice = cfg.value("payment/min_price", 5.00).toDouble();
  conf.maxPrice = cfg.value("payment/max_price", 999999.00).toDouble();
  conf.minCount = cfg.value("payment/min_count", 1).toInt();
  conf.maxCount = cfg.value("payment/max_count", 10).toInt();
  conf.currency = cfg.value("payment/currency", "€").toByteArray();
  conf.maxInteger = cfg.value("payment/max_integer", 9999999).toInt();
  config = conf;

  setColumnCount(p_tableRecord.count());
  m_header = new PurchaseTableHeader(this);
  setHorizontalHeader(m_header);
  setHeaders();
}

void PurchaseTable::setHeaders() {
  Qt::ItemFlags headerflags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                            Qt::ItemNeverHasChildren);
  QList<int> iconColumn = m_header->editableColumns();
  for (int c = 0; c < p_tableRecord.count(); c++) {
    QString fieldName = p_tableRecord.field(c).name();
    QTableWidgetItem *item = new QTableWidgetItem(QTableWidgetItem::UserType);
    item->setText(m_header->columnTitle(fieldName));
    if (iconColumn.contains(c)) {
      item->setData(Qt::DecorationRole, QIcon("://icons/edit.png"));
    }
    item->setFlags(headerflags);
    setHorizontalHeaderItem(c, item);
  }
}

QTableWidgetItem *PurchaseTable::stringItem(const QSqlField &field) const {
  QString text = field.value().toString().trimmed();
  QTableWidgetItem *item = new QTableWidgetItem(QTableWidgetItem::UserType);
  if (field.name() == "a_modified") {
    QDateTime dt = QDateTime::fromString(text, Qt::ISODate);
    item->setText(dt.toString(ANTIQUACRM_DATE_DISPLAY));
  } else {
    item->setText(text);
  }
  return item;
}

const QVariant PurchaseTable::getStringItem(int row, int column) const {
  QTableWidgetItem *m_item = item(row, column);
  return (m_item != nullptr) ? m_item->text() : QString();
}

QSpinBox *PurchaseTable::integerItem(const QSqlField &field) const {
  QSpinBox *item = new QSpinBox();
  item->setButtonSymbols(QAbstractSpinBox::NoButtons);
  item->setReadOnly(true);
  if (field.name() == "a_count") {
    item->setRange(config.minCount, config.maxCount);
  } else {
    item->setRange(0, config.maxInteger);
  }
  item->setValue(field.value().toInt());
  return item;
}

qint64 PurchaseTable::getIntegerItem(int row, int column) const {
  qint64 retval = -1;
  QSpinBox *c = qobject_cast<QSpinBox *>(cellWidget(row, column));
  if (c != nullptr)
    retval = c->value();

  return retval;
}

QDoubleSpinBox *PurchaseTable::priceItem(const QSqlField &field) const {
  QDoubleSpinBox *item = new QDoubleSpinBox();
  if (field.name() != "a_sell_price") {
    item->setButtonSymbols(QAbstractSpinBox::NoButtons);
    item->setReadOnly(true);
  }
  item->setSuffix(config.currency);
  item->setRange(config.minPrice, config.maxPrice);
  item->setValue(field.value().toDouble());
  return item;
}

double PurchaseTable::getPriceItem(int row, int column) const {
  double retval = 0.00;
  QDoubleSpinBox *c = qobject_cast<QDoubleSpinBox *>(cellWidget(row, column));
  if (c != nullptr)
    retval = c->value();

  return retval;
}

SelectArticleType *PurchaseTable::typeItem(const QSqlField &field) const {
  SelectArticleType *item = new SelectArticleType();
  item->setValue(field.value().toInt());
  return item;
}

int PurchaseTable::getTypeItem(int row, int column) const {
  SelectArticleType *c =
      qobject_cast<SelectArticleType *>(cellWidget(row, column));
  if (c != nullptr)
    return c->value().toInt();

  return 0;
}

const QString PurchaseTable::createSqlUpdate(int row, qint64 id) {
  QStringList fields;
  QStringList ignore({"a_payment_id", "a_modified"});
  for (int c = 0; c < columnCount(); c++) {
    QSqlField f = p_tableRecord.field(c);
    QString buffer;
    if (ignore.contains(f.name()))
      continue;

    if (f.name() == "a_type") {
      int i = getTypeItem(row, c);
      buffer = f.name() + "=" + QString::number(i);
      fields.append(buffer);
    } else if (f.name().contains("_price")) {
      double d = getPriceItem(row, c);
      buffer = f.name() + "=" + QString::number(d);
      fields.append(buffer);
    } else if (f.type() == QVariant::Int) {
      qint64 i = getIntegerItem(row, c);
      buffer = f.name() + "=" + QString::number(i);
      fields.append(buffer);
    } else {
      buffer = f.name() + "='" + getStringItem(row, c).toString() + "'";
      fields.append(buffer);
    }
  }
  QString sql("UPDATE " + tableName + " SET ");
  sql.append(fields.join(","));
  sql.append(",a_modified=CURRENT_TIMESTAMP WHERE");
  sql.append(" a_payment_id=" + QString::number(id) + ";");
  return sql;
}

const QString PurchaseTable::createSqlInsert(int row) {
  QStringList fields;
  QStringList values;
  QStringList ignore({"a_payment_id", "a_modified"});
  for (int c = 0; c < columnCount(); c++) {
    QSqlField f = p_tableRecord.field(c);
    if (ignore.contains(f.name()))
      continue;

    fields << f.name();
    if (f.name() == "a_type") {
      values << QString::number(getTypeItem(row, c));
    } else if (f.name().contains("_price")) {
      values << QString::number(getPriceItem(row, c));
    } else if (f.type() == QVariant::Int) {
      values << QString::number(getIntegerItem(row, c));
    } else {
      QString buffer("'" + getStringItem(row, c).toString() + "'");
      values << buffer;
    }
  }
  QString sql("INSERT INTO " + tableName + " (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(values.join(","));
  sql.append(");");
  return sql;
}

qint64 PurchaseTable::getPaymentId(int row) {
  qint64 out = -1;
  QSpinBox *m = qobject_cast<QSpinBox *>(cellWidget(row, 0));
  if (m == nullptr) {
    out = 0;
  } else if (m != nullptr && m->value() < 1) {
    out = 0;
  } else if (m != nullptr && m->value() > 0) {
    out = m->value();
  }
  return out;
}

bool PurchaseTable::removeTableRow(int row) {
  qint64 pid = getPaymentId(row);
  if (pid > 0) {
    if (rowCount() == (row + 1))
      return false;

    QString sql("DELETE FROM " + tableName + " WHERE a_payment_id=");
    sql.append(QString::number(pid) + ";");
    m_sql->query(sql);
    return m_sql->lastError().isEmpty();
  } else {
    removeRow(row);
    return true;
  }
  return false;
}

void PurchaseTable::contextMenuEvent(QContextMenuEvent *event) {
  const QModelIndex index = indexAt(event->pos());
  if (index.isValid()) {
    QMenu *m = new QMenu("Actions", this);
    QAction *ac_del =
        m->addAction(QIcon("://icons/db_remove.png"), tr("Remove"));
    connect(ac_del, SIGNAL(triggered()), SLOT(removeCurrentRow()));
    m->exec(event->globalPos());
    delete m;
  }
}

void PurchaseTable::removeCurrentRow() {
  int row = -1;
  QModelIndex index;
  foreach (QModelIndex i, selectedIndexes()) {
    row = i.row();
    break;
  }

  QString info("<p>");
  info.append(tr("Do you really want to remove this Entry from the list?"));
  info.append("</p><p>");
  info.append(tr("This operation is not reversible!"));
  info.append("</p><p>");
  info.append(tr("If no SQL Database Payment Id exists, it will fail!"));
  info.append("</p>");
  int ret = QMessageBox::question(this, tr("Remove Article"), info);
  if (ret == QMessageBox::Yes) {
    if (removeTableRow(row)) {
      if(save()) {
        clearContents();
        setRowCount(0);
        sqlQueryTable(history.second, history.first);
      }
    }
  }
}

void PurchaseTable::sqlQueryTable(qint64 id, const QString &field) {
  if (id < 1 || field.isEmpty())
    return;

  history.first = field;
  history.second = id;

  QString sql("SELECT * FROM " + tableName + " WHERE ");
  sql.append(field + "=" + QString::number(id) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int r = rowCount();
      setRowCount((rowCount() + 1));
      for (int c = 0; c < p_tableRecord.count(); c++) {
        QSqlField f = p_tableRecord.field(c);
        f.setValue(q.value(f.name()));
        if (f.name() == "a_type") {
          setCellWidget(r, c, typeItem(f));
        } else if (f.name().contains("_price")) {
          setCellWidget(r, c, priceItem(f));
        } else if (f.type() == QVariant::Int) {
          setCellWidget(r, c, integerItem(f));
        } else {
          setItem(r, c, stringItem(f));
        }
      }
    }
  }
}

bool PurchaseTable::save() {
  QStringList sqlQueries;
  for (int r = 0; r < rowCount(); r++) {
    qint64 pid = getPaymentId(r);
    if (pid > 0)
      sqlQueries << createSqlUpdate(r, pid);
    else
      sqlQueries << createSqlInsert(r);
  }

  if (sqlQueries.count() < 1) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "INSERT|UPDATE not created.";
#endif
    return false;
  }

  m_sql->query(sqlQueries.join("\n"));
  if (m_sql->lastError().isEmpty()) {
    return true;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
  qWarning("SQL Error in PurchaseTable save.");
#endif

  return false;
}

bool PurchaseTable::setRequiredIds(qint64 oId, qint64 cId) {
  QStringList fields({"a_order_id", "a_customer_id"});
  for (int r = 0; r < rowCount(); r++) {
    for (int c = 0; c < p_tableRecord.count(); c++) {
      QSqlField f = p_tableRecord.field(c);
      if (!fields.contains(f.name()))
        continue;

      QSpinBox *m = qobject_cast<QSpinBox *>(cellWidget(r, c));
      if (f.name() == "a_order_id") {
        m->setValue(oId);
      } else if (f.name() == "a_customer_id") {
        m->setValue(cId);
      }
    }
  }
  return true;
}

void PurchaseTable::hideColumns(const QList<int> columns) {
  QListIterator<int> it(columns);
  while (it.hasNext()) {
    int i = it.next();
    if (i >= 0) {
      m_header->setSectionHidden(i, true);
    }
  }
  m_header->setStretchLastSection(true);
}

bool PurchaseTable::addRow(const AntiquaCRM::OrderArticleItems &items) {
  int r = rowCount();
  setRowCount((r + 1));
  QListIterator<AntiquaCRM::ArticleOrderItem> it(items);
  while (it.hasNext()) {
    AntiquaCRM::ArticleOrderItem item = it.next();
    int c = p_tableRecord.indexOf(item.key);
    QSqlField f = p_tableRecord.field(c);
    f.setValue(item.value);
    if (f.name() == "a_type") {
      setCellWidget(r, c, typeItem(f));
    } else if (f.name().contains("_price")) {
      setCellWidget(r, c, priceItem(f));
    } else if (f.type() == QVariant::Int) {
      setCellWidget(r, c, integerItem(f));
    } else {
      setItem(r, c, stringItem(f));
    }
  }
  return false;
}

const AntiquaCRM::OrderArticleItems PurchaseTable::getRow(int row) {
  AntiquaCRM::OrderArticleItems items;
  for (int r = 0; r < rowCount(); r++) {
    if (r == row) {
      for (int c = 0; c < columnCount(); c++) {
        QSqlField f = p_tableRecord.field(c);
        AntiquaCRM::ArticleOrderItem item;
        item.key = f.name();
        if (f.name() == "a_type") {
          item.value = getTypeItem(r, c);
        } else if (f.name().contains("_price")) {
          item.value = getPriceItem(r, c);
        } else if (f.type() == QVariant::Int) {
          item.value = getIntegerItem(r, c);
        } else {
          item.value = getStringItem(r, c);
        }
        items.append(item);
      }
    }
  }
  return items;
}
