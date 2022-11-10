// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersitemlist.h"
#include "orderspaymenttable.h"

#include <QDebug>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSizePolicy>

OrdersItemList::OrdersItemList(QWidget *parent) : QWidget{parent} {
  setObjectName("orders_item_list");
  setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_cfg = new AntiquaCRM::ASettings(this);

  minPrice = m_cfg->value("payment/min_price", 5.00).toDouble();
  maxPrice = m_cfg->value("payment/max_price", 999999.00).toDouble();
  currency = m_cfg->value("payment/currency", "€").toByteArray();

  QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Minimum);

  m_table = new OrdersPaymentTable(this);
  layout->addWidget(m_table);

  QGroupBox *m_groupBox = new QGroupBox(this);
  m_groupBox->setTitle(tr("Add Article orders") + ":");

  QGridLayout *groupLayout = new QGridLayout(m_groupBox);
  groupLayout->setColumnStretch(0, 1);
  groupLayout->setColumnStretch(1, 1);

  QHBoxLayout *searchLayout = new QHBoxLayout();
  QLabel *info = new QLabel(this);
  info->setText(tr("Add here the article Ids for this order:"));
  searchLayout->addWidget(info);
  m_insertID = new QSpinBox(this);
  m_insertID->setRange(1, (qint64)maxPrice);
  m_insertID->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_insertID->clear();
  searchLayout->addWidget(m_insertID);
  groupLayout->addLayout(searchLayout, 0, 0, 1, 2, Qt::AlignRight);

  QPushButton *btn_check = new QPushButton(m_groupBox);
  btn_check->setText(tr("Check"));
  btn_check->setIcon(QIcon("://icons/db_update.png"));
  btn_check->setSizePolicy(policy);
  groupLayout->addWidget(btn_check, 0, 2, 1, 1);

  // INFO
  m_searchInfo = new QLineEdit(m_groupBox);
  m_searchInfo->setReadOnly(true);
  m_searchInfo->setPlaceholderText(tr("Info about Article found by input ..."));
  m_searchInfo->setSizePolicy(policy);
  groupLayout->addWidget(m_searchInfo, 1, 0, 1, 2);

  QPushButton *btn_add = new QPushButton(m_groupBox);
  btn_add->setText(tr("Insert Article"));
  btn_add->setIcon(QIcon("://icons/db_add.png"));
  btn_add->setSizePolicy(policy);
  btn_add->setMinimumWidth(150);
  groupLayout->addWidget(btn_add, 1, 2, 1, 1);
  // INFO

  m_groupBox->setLayout(groupLayout);
  layout->addWidget(m_groupBox);

  layout->addStretch(1);
  setLayout(layout);

  // connect(btn_add, SIGNAL(clicked()), SLOT(insertArticle()));
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
}

void OrdersItemList::clearTable() {
  m_table->clearContents();
  m_table->setRowCount(0);
}

void OrdersItemList::insertArticle(const AntiquaCRM::OrderArticleItems &item) {
  if (item.size() < 1)
    return;

  int r = m_table->rowCount();
  m_table->setRowCount((m_table->rowCount() + 1));
  QListIterator<AntiquaCRM::ArticleOrderItem> it(item);
  while (it.hasNext()) {
    AntiquaCRM::ArticleOrderItem article = it.next();
    if (article.key == "a_provider_id") {
      m_table->setItem(r, 0, createItem(article.value.toString()));
    }
    if (article.key == "a_article_id") {
      qint64 aId = article.value.toInt();
      m_table->setItem(r, 1, createItem(QString::number(aId)));
    }
    if (article.key == "a_price") {
      m_table->setCellWidget(r, 2, addPrice(article.value.toDouble(), r));
    }
    if (article.key == "a_sell_price") {
      m_table->setCellWidget(r, 3, addSellPrice(article.value.toDouble(), r));
    }
    if (article.key == "a_count") {
      m_table->setCellWidget(r, 4, addCount(article.value.toInt(), r));
    }
    if (article.key == "a_type") {
      AntiquaCRM::ArticleType t =
          static_cast<AntiquaCRM::ArticleType>(article.value.toInt());
      QString txt;
      if (t == AntiquaCRM::ArticleType::BOOK)
        txt = tr("Book");
      else if (t == AntiquaCRM::ArticleType::MEDIA)
        txt = tr("Media");
      else if (t == AntiquaCRM::ArticleType::PRINTS)
        txt = tr("Prints");
      else
        txt = tr("Unknown");

      m_table->setItem(r, 5, createItem(txt));
    }
    if (article.key == "a_title") {
      m_table->setItem(r, 6, createItem(article.value.toString()));
    }
  }
  clearSearchInput();
  setModified(true);
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

bool OrdersItemList::importPayments(
    const QList<AntiquaCRM::OrderArticleItems> &list) {
  QListIterator<AntiquaCRM::OrderArticleItems> it(list);
  while (it.hasNext()) {
    insertArticle(it.next());
  }
  return true;
}

const QHash<QString, QVariant> OrdersItemList::getTableRow(int row) {
  QHash<QString, QVariant> list;
  qDebug() << Q_FUNC_INFO << "TODO";
  list.insert("a_payment_id", m_table->item(row, 0)->text().toInt());
  list.insert("a_order_id", 0);
  list.insert("a_article_id", m_table->item(row, 1)->text().toInt());
  list.insert("a_customer_id", 0);
  list.insert("a_price", getPrice(row));
  list.insert("a_sell_price", getSellPrice(row));
  list.insert("a_count", getCount(row));
  list.insert("a_type", m_table->item(row, 5)->text());
  list.insert("a_title", m_table->item(row, 6)->text());
  return list;
}
