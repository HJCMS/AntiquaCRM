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

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 1, 0, 1);
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 1);

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

  QLabel *info = new QLabel(this);
  info->setText(tr("Add here the article Ids for this order:"));
  layout->addWidget(info, 2, 0, 1, 1, Qt::AlignRight);

  m_insertID = new QSpinBox(this);
  m_insertID->setRange(1, 99999999);
  layout->addWidget(m_insertID, 2, 1, 1, 1);

  QPushButton *btn_check = new QPushButton(this);
  btn_check->setText(tr("Check"));
  btn_check->setIcon(myIcon("db_update"));
  btn_check->setSizePolicy(policy);
  layout->addWidget(btn_check, 2, 2, 1, 1);

  setLayout(layout);

  connect(btn_add, SIGNAL(clicked()), this, SLOT(insertArticle()));
  connect(btn_check, SIGNAL(clicked()), this, SLOT(createSignal()));
}

QDoubleSpinBox *OrdersItemList::addPriceBox(double val, bool edit) {
  QDoubleSpinBox *p = new QDoubleSpinBox(m_table);
  p->setReadOnly(edit);
  p->setValue(val);
  p->setSuffix("€");
  return p;
}

QSpinBox *OrdersItemList::addCountBox(const QVariant &val) {
  int count = val.toInt();
  QSpinBox *c = new QSpinBox(m_table);
  c->setMinimum(1);
  c->setMaximum(count);
  // c->setReadOnly((count > 1));
  c->setValue(count);
  return c;
}

QTableWidgetItem *OrdersItemList::createItem(const QVariant &val) {
  QString str(val.toString());
  QTableWidgetItem *item = new QTableWidgetItem(str.trimmed());
  return item;
}

void OrdersItemList::addTableRow() {
  int r = m_table->rowCount();
  m_table->setRowCount((m_table->rowCount() + 1));
  m_table->setItem(r, 0, createItem(p_article.articleId));
  m_table->setCellWidget(r, 1, addPriceBox(p_article.price, true));
  m_table->setCellWidget(r, 2, addPriceBox(p_article.price));
  m_table->setCellWidget(r, 3, addCountBox(p_article.count));
  m_table->setItem(r, 4, createItem(p_article.title));
  clearInput();
}

void OrdersItemList::insertArticle() {
  for (int r = 0; r < m_table->rowCount(); r++) {
    if (m_table->getArticleId(r) == p_article.articleId) {
      emit statusMessage(tr("Duplicate Entry"));
      return;
    }
  }
  addTableRow();
}

void OrdersItemList::createSignal() {
  int id = m_insertID->value();
  if (id < 1)
    return;

  m_searchInfo->clear();
  emit searchArticle(id);
}

void OrdersItemList::clearInput() {
  m_insertID->clear();
  m_searchInfo->clear();
}

void OrdersItemList::foundArticle(const Article &found) {
  QString buffer(found.summary.trimmed());
  if (!buffer.isEmpty()) {
    m_searchInfo->setText(buffer);
    p_article = found;
  }
}
