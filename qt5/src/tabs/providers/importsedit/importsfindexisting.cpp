// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "importsfindexisting.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QHeaderView>
#include <QLayout>
#include <QTableWidgetItem>

ImportsFindExisting::ImportsFindExisting(QWidget *parent) : QWidget{parent} {
  setContentsMargins(2, 2, 2, 2);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  // BEGIN::Search
  QGroupBox *search_group = new QGroupBox(tr("Search existing Customer"), this);
  QHBoxLayout *search_layout = new QHBoxLayout(search_group);
  m_firstname = new LineEdit(this);
  m_firstname->setObjectName("c_firstname");
  m_firstname->setInfo(tr("Firstname"));
  search_layout->addWidget(m_firstname);
  m_lastname = new LineEdit(this);
  m_lastname->setObjectName("c_lastname");
  m_lastname->setInfo(tr("Lastname"));
  search_layout->addWidget(m_lastname);
  QPushButton *btn_search = new QPushButton(tr("Search"), this);
  search_layout->addWidget(btn_search);
  layout->addWidget(search_group);
  // End::Search

  // BEGIN::Found
  m_table = new QTableWidget(this);
  QStringList _labels({tr("Id"), tr("Fullname"), tr("Adress")});
  m_table->setColumnCount(_labels.size());
  m_table->setHorizontalHeaderLabels(_labels);
  m_table->setToolTip(tr("Current article purchases"));
  m_table->setSortingEnabled(false);
  m_table->setAlternatingRowColors(true);
  m_table->setCornerButtonEnabled(false);
  m_table->setDragEnabled(false);
  m_table->setDragDropOverwriteMode(false);
  m_table->setWordWrap(false);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QHeaderView *_view = m_table->horizontalHeader();
  _view->setSectionResizeMode(QHeaderView::ResizeToContents);
  _view->setStretchLastSection(true);
  layout->addWidget(m_table);
  // End::Found

  btn_next = new QPushButton(tr("Next"), this);
  btn_next->setEnabled(false);
  layout->addWidget(btn_next, 0, Qt::AlignRight);

  layout->addStretch();
  setLayout(layout);

  // Signal
  connect(btn_search, SIGNAL(clicked()), SLOT(prepareSearch()));
  connect(btn_next, SIGNAL(clicked()), SIGNAL(sendNextPage()));
  connect(m_table, SIGNAL(cellClicked(int, int)), SLOT(itemClicked(int, int)));
}

QLabel *ImportsFindExisting::info(const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
}

void ImportsFindExisting::itemClicked(int row, int column) {
  Q_UNUSED(column);
  btn_next->setEnabled((row >= 0));
  emit sendUseClause("c_id=" + m_table->item(row, 0)->text());
}

void ImportsFindExisting::prepareSearch() {
  QString _query("(c_firstname ILIKE '");
  _query.append(m_firstname->value().toString());
  _query.append("' AND c_lastname ILIKE '");
  _query.append(m_lastname->value().toString());
  _query.append("')");
  emit sendFindClause(_query);
}

void ImportsFindExisting::clear() {
  m_firstname->reset();
  m_lastname->reset();
  m_table->clearContents();
}

void ImportsFindExisting::addCustomer(const QJsonObject &customer) {
  m_table->insertRow(m_table->rowCount());

  int _row = (m_table->rowCount() - 1);
  QTableWidgetItem *_item = new QTableWidgetItem(QTableWidgetItem::UserType);
  _item->setText(QString::number(customer.value("c_id").toInt()));
  m_table->setItem(_row, 0, _item);

  _item = new QTableWidgetItem(QTableWidgetItem::UserType);
  QString _name(customer.value("c_firstname").toString());
  _name.append(" " + customer.value("c_lastname").toString());
  _item->setText(_name);
  m_table->setItem(_row, 1, _item);

  _item = new QTableWidgetItem(QTableWidgetItem::UserType);
  QString _address = customer.value("c_postal_address").toString().trimmed();
  _item->setText(AntiquaCRM::AUtil::trim(_address));
  m_table->setItem(_row, 2, _item);
}
