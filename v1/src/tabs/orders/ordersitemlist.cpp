// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersitemlist.h"
#include "purchasetable.h"

#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSizePolicy>

OrdersItemList::OrdersItemList(QWidget *parent) : QWidget{parent} {
  setObjectName("orders_item_list");
  setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  QString groupInfo = tr("Insert a new Article with Id to this order");

  m_addArticleGroup = new QGroupBox(this);
  m_addArticleGroup->setTitle(groupInfo + ":  ");
  QHBoxLayout *groupLayout = new QHBoxLayout(m_addArticleGroup);
  groupLayout->setContentsMargins(2, 2, 2, 2);
  groupLayout->addStretch(1);
  m_notifier = new QLabel(m_addArticleGroup);
  m_notifier->setStyleSheet("QLabel {color:red;font-weight:bold;}");
  groupLayout->addWidget(m_notifier, Qt::AlignLeft);
  m_insertID = new IntSpinBox(m_addArticleGroup);
  m_insertID->setRange(0, 99999999);
  m_insertID->setShowButtons(false);
  m_insertID->setInfo(tr("Insert here the Article ID."));
  groupLayout->addWidget(m_insertID);
  QPushButton *btn_add = new QPushButton(m_addArticleGroup);
  btn_add->setText(tr("Insert Article"));
  btn_add->setIcon(QIcon("://icons/db_add.png"));
  btn_add->setSizePolicy(policy);
  groupLayout->addWidget(btn_add);
  m_addArticleGroup->setLayout(groupLayout);
  layout->addWidget(m_addArticleGroup);

  QString tbInfo = tr("Current Article orders");
  layout->addWidget(new QLabel(tbInfo + ":", this));

  m_table = new PurchaseTable(this, false);
  // Für den Kunden nicht anzeigen!
  m_table->hideColumns(QList<int>({0, 1, 3, 8, 9}));
  m_table->setEnabled(false);
  layout->addWidget(m_table);

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_add, SIGNAL(clicked()), SLOT(setSearchArticle()));
}

void OrdersItemList::setSearchArticle() {
  qint64 id = m_insertID->value().toInt();
  if (id < 1) {
    setAlertMessage(tr("Empty entries are cowardly refused!"));
    return;
  }
  setAlertMessage();
  emit searchArticleById(id);
}

void OrdersItemList::clearSearchInput() {
  m_insertID->reset();
  m_notifier->clear();
}

void OrdersItemList::clearTable() {
  m_table->clearTable();
  clearSearchInput();
}

void OrdersItemList::setAlertMessage(const QString &message) {
  if (message.length() > 0)
    m_notifier->setText(message);
  else
    m_notifier->clear();
}

void OrdersItemList::insertArticle(const AntiquaCRM::OrderArticleItems &item) {
  m_table->addOrderArticle(item);
}

void OrdersItemList::insertSearchId(int articleId) {
  if (articleId >= 0)
    m_insertID->setValue(articleId);
}

bool OrdersItemList::setData(const QList<AntiquaCRM::OrderArticleItems> &list) {
  bool status = m_table->setOrderArticles(list);
  m_table->setEnabled(status);
  return status;
}

const QStringList OrdersItemList::getQueryData() {
  return m_table->getQueryArticles();
}
