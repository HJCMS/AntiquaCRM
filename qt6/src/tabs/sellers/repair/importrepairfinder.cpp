// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "importrepairfinder.h"

ImportRepairFinder::ImportRepairFinder(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  // BEGIN::Search
  QGroupBox *m_sgroup = new QGroupBox(tr("Search existing Customer"), this);
  QHBoxLayout *m_slayout = new QHBoxLayout(m_sgroup);
  m_slayout->setContentsMargins(2, 2, 2, 2);
  m_firstname = new AntiquaCRM::TextLine(m_sgroup);
  m_firstname->setObjectName("c_firstname");
  m_firstname->setBuddyLabel(tr("Firstname"));
  m_slayout->addWidget(m_firstname);
  m_lastname = new AntiquaCRM::TextLine(m_sgroup);
  m_lastname->setObjectName("c_lastname");
  m_lastname->setBuddyLabel(tr("Lastname"));
  m_slayout->addWidget(m_lastname);

  QPushButton *btn_search = new QPushButton(tr("Search"), m_sgroup);
  btn_search->setIcon(AntiquaCRM::antiquaIcon("action-search"));
  m_slayout->addWidget(btn_search);

  layout->addWidget(m_sgroup);
  // End::Search

  // BEGIN::Found
  const QString _info =
      tr("Result of current customer search in the system database.");
  QGroupBox *m_fgroup = new QGroupBox(_info, this);
  QVBoxLayout *m_flayout = new QVBoxLayout(m_fgroup);
  m_flayout->setContentsMargins(2, 2, 2, 2);
  m_table = new QTableWidget(m_fgroup);
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

  m_flayout->addWidget(m_table);
  m_fgroup->setLayout(m_flayout);
  layout->addWidget(m_fgroup);
  // End::Found

  layout->addStretch();
  setLayout(layout);

  // Signal
  connect(m_lastname, SIGNAL(signalEnterPressed()), SLOT(prepareSearch()));
  connect(btn_search, SIGNAL(clicked()), SLOT(prepareSearch()));
  connect(m_table, SIGNAL(cellClicked(int, int)), SLOT(itemClicked(int, int)));
}

QLabel *ImportRepairFinder::info(const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
}

void ImportRepairFinder::itemClicked(int row, int column) {
  Q_UNUSED(column);
  emit sendUseClause("c_id=" + m_table->item(row, 0)->text());
}

void ImportRepairFinder::prepareSearch() {
  QString _query("(c_firstname ILIKE '");
  _query.append(m_firstname->getValue().toString());
  _query.append("' AND c_lastname ILIKE '");
  _query.append(m_lastname->getValue().toString());
  _query.append("%')");
  emit sendFindClause(_query);
}

void ImportRepairFinder::clear() {
  m_firstname->reset();
  m_lastname->reset();
  m_table->clearContents();
  m_table->setRowCount(0);
}

void ImportRepairFinder::addCustomer(const QJsonObject &customer) {
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
