// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "changecustomerfind.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QPainter>
#include <QRegularExpression>

ChangeCustomerTable::ChangeCustomerTable(QWidget* parent) : QTableWidget{parent} {
  setColumnCount(5);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Id"), QTableWidgetItem::UserType));
  setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Organisation"), QTableWidgetItem::UserType));
  setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Fullname"), QTableWidgetItem::UserType));
  setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Imported name"), QTableWidgetItem::UserType));
  setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Address info"), QTableWidgetItem::UserType));
  horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  horizontalHeader()->setStretchLastSection(true);
}

void ChangeCustomerTable::paintEvent(QPaintEvent* event) {
  if (rowCount() == 0) {
    const QString _info(tr("For preview, double click on listed items."));
    QPainter _painter(viewport());
    _painter.setBrush(palette().text());
    _painter.setFont(font());
    _painter.setOpacity(0.8);
    _painter.drawText(rect(), Qt::AlignCenter, _info);
  }
  QTableView::paintEvent(event);
}

ChangeCustomerFind::ChangeCustomerFind(QWidget* parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  int _r = 0;
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);

  const QIcon _sicon = AntiquaCRM::antiquaIcon("view-search");

  m_comboBox = new AntiquaCRM::AComboBox(this);
  m_comboBox->insertItem(0, tr("Full name"), // multible search
                         QStringList({"c_firstname", "c_lastname"}));
  m_comboBox->setItemData(0, _sicon, Qt::DecorationRole);
  m_comboBox->setItemData(0, tr("Search for first and second name."), Qt::ToolTipRole);

  m_comboBox->insertItem(1, tr("Name of company"), "c_company_name");
  m_comboBox->setItemData(1, _sicon, Qt::DecorationRole);

  m_comboBox->insertItem(2, tr("Forename"), "c_firstname");
  m_comboBox->setItemData(2, _sicon, Qt::DecorationRole);

  m_comboBox->insertItem(3, tr("Surname"), "c_lastname");
  m_comboBox->setItemData(3, _sicon, Qt::DecorationRole);

  m_comboBox->insertItem(4, tr("Customer ID"), "c_id");
  m_comboBox->setItemData(4, _sicon, Qt::DecorationRole);

  m_comboBox->insertItem(5, tr("Provider imported name"), "c_provider_import");
  m_comboBox->setItemData(5, _sicon, Qt::DecorationRole);
  m_comboBox->setItemData(5, tr(""), Qt::ToolTipRole);

  layout->addWidget(m_comboBox, _r, 0, 1, 1);

  m_searchLine = new AntiquaCRM::TextLine(this);
  m_searchLine->setBuddyLabel(tr("Search"));
  layout->addWidget(m_searchLine, _r, 1, 1, 1);

  QPushButton* btn_search = new QPushButton(tr("Search"), this);
  btn_search->setIcon(AntiquaCRM::antiquaIcon("system-search"));
  btn_search->setToolTip(tr("Push to start search."));
  layout->addWidget(btn_search, _r++, 2, 1, 1, Qt::AlignLeft);

  m_tableWidget = new ChangeCustomerTable(this);
  layout->addWidget(m_tableWidget, _r++, 0, 1, 3);

  setLayout(layout);

  // signals
  connect(btn_search, SIGNAL(clicked()), SLOT(searchClicked()));
  connect(m_searchLine, SIGNAL(signalEnterPressed()), SLOT(searchClicked()));
  connect(m_tableWidget, SIGNAL(doubleClicked(QModelIndex)), SLOT(customerSelected(QModelIndex)));
}

ChangeCustomerFind::~ChangeCustomerFind() {
  if (m_sql)
    m_sql->deleteLater();
}

QLabel* ChangeCustomerFind::labelCell(const QString& txt) {
  QLabel* _lb = new QLabel(txt, this);
  return _lb;
}

void ChangeCustomerFind::customerSelected(const QModelIndex& index) {
  if (m_tableWidget->rowCount() < 1 || index.row() < 0)
    return;

  qint64 _id = m_tableWidget->item(index.row(), 0)->data(Qt::EditRole).toInt();
  emit sendCustomerSelected(_id);
}

void ChangeCustomerFind::searchClicked() {
  QString _search = m_searchLine->getValue().toString().trimmed();
  if (_search.length() < 1)
    return;

  QStringList _fields;
  QVariant _selected = m_comboBox->itemData(m_comboBox->currentIndex());
  if (_selected.metaType().id() == QMetaType::QStringList) {
    _fields << _selected.toStringList();
  } else {
    _fields << _selected.toString().trimmed().toLower();
  }

  QString _sql("SELECT c_id, c_company_name, ");
  _sql.append("CONCAT(c_firstname, ' ', c_lastname) AS name,");
  _sql.append("c_provider_import AS import,");
  _sql.append("CONCAT(c_postalcode, ' ', c_location, ' ', c_street) AS address");
  _sql.append(" FROM customers WHERE ");
  if (_fields.contains("c_id")) {
    static const QRegularExpression _pattern("\\D");
    _search = _search.replace(_pattern, "");
    if (_search.length() < 1)
      return;

    _sql.append("c_id=");
    _sql.append(_search);
  } else if (_fields.size() > 1) {
    QStringList _list = _search.split(" ");
    QStringList _statements;
    foreach (QString _f, _fields) {
      QString _buffer(_f);
      _buffer.append(" ILIKE '%");
      _buffer.append(_list.join("%"));
      _buffer.append("%'");
      _statements.append(_buffer);
    }
    _sql.append("(");
    _sql.append(_statements.join(" OR "));
    _sql.append(")");
  } else {
    QStringList _list = _search.split(" ");
    _sql.append(_fields.join("").trimmed());
    _sql.append(" ILIKE '%");
    _sql.append(_list.join("%"));
    _sql.append("%'");
  }
  _sql.append(" ORDER BY c_id LIMIT 99;");

  m_tableWidget->clearContents();
  m_tableWidget->setRowCount(0);

  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql != nullptr) {
    QSqlQuery _q = m_sql->query(_sql);
    if (_q.size() < 1) {
      emit sendNotification(tr("Nothing found."));
      return;
    }

    // start output
    qint32 _row = 0;
    m_tableWidget->setRowCount(_q.size());
    QSqlRecord _r = _q.record();
    while (_q.next()) {
      int _col = 0;
      for (int i = 0; i < _r.count(); i++) {
        QTableWidgetItem* m_i =
            new QTableWidgetItem(_q.value(i).toString(), QTableWidgetItem::UserType);
        m_i->setFlags(m_i->flags() & ~Qt::ItemIsEditable);
        m_tableWidget->setItem(_row, _col++, m_i);
      }
      _row++;
    }
  }
}
