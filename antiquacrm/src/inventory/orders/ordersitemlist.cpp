// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersitemlist.h"
#include "myicontheme.h"
#include "orderpaymentstable.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSizePolicy>
#include <QTableWidgetItem>

OrdersItemList::OrdersItemList(QWidget *parent) : QWidget{parent} {
  setObjectName("orders_item_list");

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 1, 0, 1);
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 1);

  m_cfg = new ApplSettings(this);

  minPrice = m_cfg->value("payment/min_price", 5.00).toDouble();
  maxPrice = m_cfg->value("payment/max_price", 999999.00).toDouble();
  currency = m_cfg->value("payment/currency", "€").toByteArray();

  QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Minimum);

  m_table = new OrderPaymentsTable(this);
  layout->addWidget(m_table, 0, 0, 1, 3);

  m_searchInfo = new QLineEdit(this);
  m_searchInfo->setReadOnly(true);
  m_searchInfo->setPlaceholderText(tr("Info about Article found by input ..."));
  m_searchInfo->setSizePolicy(policy);
  layout->addWidget(m_searchInfo, 1, 0, 1, 2);

  QPushButton *btn_add = new QPushButton(this);
  btn_add->setText(tr("Insert Article"));
  btn_add->setIcon(myIcon("db_add"));
  btn_add->setSizePolicy(policy);
  btn_add->setMinimumWidth(150);
  layout->addWidget(btn_add, 1, 2, 1, 1);

  QHBoxLayout *searchLayout = new QHBoxLayout();
  QLabel *info = new QLabel(this);
  info->setText(tr("Add here the article Ids for this order:"));
  searchLayout->addWidget(info);
  m_insertID = new QSpinBox(this);
  m_insertID->setRange(1, 99999999);
  m_insertID->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_insertID->clear();
  searchLayout->addWidget(m_insertID);
  layout->addLayout(searchLayout, 2, 0, 1, 2, Qt::AlignRight);

  QPushButton *btn_check = new QPushButton(this);
  btn_check->setText(tr("Check"));
  btn_check->setIcon(myIcon("db_update"));
  btn_check->setSizePolicy(policy);
  layout->addWidget(btn_check, 2, 2, 1, 1);

  setLayout(layout);

  connect(btn_add, SIGNAL(clicked()), this, SLOT(insertArticle()));
  connect(btn_check, SIGNAL(clicked()), this, SLOT(createSearchSignal()));
  connect(m_table, SIGNAL(s_removeTableRow(int)), this,
          SIGNAL(askToRemoveRow(int)));
  connect(m_table, SIGNAL(hasModified(bool)), this, SIGNAL(hasModified(bool)));
}

QDoubleSpinBox *OrdersItemList::addPrice(double val, int row) {
  QDoubleSpinBox *p = new QDoubleSpinBox(m_table);
  p->setObjectName("a_price#" + QString::number(row));
  p->setButtonSymbols(QAbstractSpinBox::NoButtons);
  p->setReadOnly(true);
  p->setSuffix(currency);
  p->setRange(minPrice, maxPrice);
  p->setValue(val);
  return p;
}

double OrdersItemList::getPrice(int row) {
  double retval = 0.00;
  QDoubleSpinBox *p =
      qobject_cast<QDoubleSpinBox *>(m_table->cellWidget(row, 2));
  if (p != nullptr)
    retval = p->value();

  return retval;
}

QDoubleSpinBox *OrdersItemList::addSellPrice(double val, int row) {
  QDoubleSpinBox *p = new QDoubleSpinBox(m_table);
  p->setObjectName("a_sell_price#" + QString::number(row));
  p->setSuffix(currency);
  p->setRange(minPrice, maxPrice);
  p->setValue(val);
  return p;
}

double OrdersItemList::getSellPrice(int row) {
  double retval = 0.00;
  QDoubleSpinBox *p =
      qobject_cast<QDoubleSpinBox *>(m_table->cellWidget(row, 3));
  if (p != nullptr)
    retval = p->value();

  return retval;
}

QSpinBox *OrdersItemList::addCount(const QVariant &val, int row, int max) {
  int count = val.toInt();
  QSpinBox *c = new QSpinBox(m_table);
  c->setObjectName("a_count#" + QString::number(row));
  c->setMinimum(1);
  c->setValue(count);
  c->setMaximum(max);
  return c;
}

int OrdersItemList::getCount(int row) {
  int retval = -1;
  QSpinBox *c = qobject_cast<QSpinBox *>(m_table->cellWidget(row, 4));
  if (c != nullptr)
    retval = c->value();

  return retval;
}

QTableWidgetItem *OrdersItemList::createItem(const QVariant &val) {
  QString str(val.toString());
  QTableWidgetItem *item = new QTableWidgetItem(str.trimmed());
  item->setFlags(Qt::ItemIsEnabled); // Kein Editieren zulassen!
  return item;
}

void OrdersItemList::addTableRow() {
  int r = m_table->rowCount();
  m_table->setRowCount((m_table->rowCount() + 1));
  m_table->setItem(r, 0, createItem(p_payments.payment()));
  m_table->setItem(r, 1, createItem(p_payments.article()));
  m_table->setCellWidget(r, 2, addPrice(p_payments.price(), r));
  m_table->setCellWidget(r, 3, addSellPrice(p_payments.sellPrice(), r));
  m_table->setCellWidget(r, 4, addCount(p_payments.count(), r));
  m_table->setItem(r, 5, createItem(p_payments.title()));
  clearSearchInput();
  setModified(true);
}

void OrdersItemList::insertArticle() {
  if (p_payments.article() <= 0) {
    QString msg(tr("Inserting an empty entry is cowardly denied."));
    msg.append("<p>");
    msg.append(tr("Please search for an entry using an item ID first."));
    msg.append("</p>");
    emit statusMessage(msg);
    return;
  }

  for (int r = 0; r < m_table->rowCount(); r++) {
    if (m_table->getArticleId(r) == p_payments.article()) {
      QString msg("<p>");
      msg.append(tr("Duplicate Entry"));
      msg.append(": " + QString::number(p_payments.article()));
      msg.append("</p>");
      emit statusMessage(msg);
      return;
    }
  }
  addTableRow();
}

void OrdersItemList::createSearchSignal() {
  int id = m_insertID->value();
  if (id < 1)
    return;

  m_searchInfo->clear();
  emit searchArticle(id);
}

void OrdersItemList::clearSearchInput() {
  m_insertID->clear();
  m_searchInfo->clear();
  p_payments.clear();
}

void OrdersItemList::clearTable() {
  m_table->clearContents();
  m_table->setRowCount(0);
}

void OrdersItemList::setModified(bool b) {
  modified = b;
  emit hasModified(modified);
}

void OrdersItemList::insertSearchId(int articleId) {
  if (articleId >= 0)
    m_insertID->setValue(articleId);
}

void OrdersItemList::removeTableRow(int row) {
  int c = m_table->rowCount();
  m_table->removeRow(row);
  m_table->setRowCount(c - 1);
}

int OrdersItemList::payments() { return m_table->rowCount(); }

void OrdersItemList::importPayments(const QList<OrderArticle> &list) {
  QListIterator<OrderArticle> it(list);
  while (it.hasNext()) {
    p_payments = it.next();
    addTableRow();
  }
}

const QHash<QString, QVariant> OrdersItemList::getTableRow(int row) {
  QHash<QString, QVariant> list;
  list.insert("a_payment_id", m_table->item(row, 0)->text().toInt());
  list.insert("a_order_id", 0);
  list.insert("a_article_id", m_table->item(row, 1)->text().toInt());
  list.insert("a_customer_id", 0);
  list.insert("a_count", getCount(row));
  list.insert("a_title", m_table->item(row, 5)->text());
  list.insert("a_price", getPrice(row));
  list.insert("a_sell_price", getSellPrice(row));
  return list;
}

void OrdersItemList::addArticleRow(const OrderArticle &set) {
  p_payments = set;
  QString buffer(p_payments.summary().trimmed());
  if (!buffer.isEmpty()) {
    m_searchInfo->setText(buffer);
  }
}
