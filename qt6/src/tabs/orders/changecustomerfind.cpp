// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "changecustomerfind.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QPushButton>
#include <QRegularExpression>

ChangeCustomerFind::ChangeCustomerFind(QWidget* parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  int _r = 0;
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);

  int _ci = 0;
  m_comboBox = new QComboBox(this);
  m_comboBox->insertItem(_ci++, tr("Name of company"), "c_company_name");
  m_comboBox->insertItem(_ci, tr("Full name"), // multible search
                         QStringList({"c_firstname", "c_lastname"}));
  m_comboBox->setItemData(_ci++, tr("Search for first and second name."), Qt::ToolTipRole);
  m_comboBox->insertItem(_ci++, tr("Forename"), "c_firstname");
  m_comboBox->insertItem(_ci++, tr("Surname"), "c_lastname");
  m_comboBox->insertItem(_ci++, tr("Customer ID"), "c_id");
  m_comboBox->insertItem(_ci, tr("Provider imported name"), "c_provider_import");
  m_comboBox->setItemData(_ci++, tr(""), Qt::ToolTipRole);
  layout->addWidget(m_comboBox, _r, 0, 1, 1);

  m_searchLine = new QLineEdit(this);
  layout->addWidget(m_searchLine, _r, 1, 1, 1);

  QPushButton* btn_search = new QPushButton(tr("Search"), this);
  btn_search->setIcon(AntiquaCRM::antiquaIcon("system-search"));
  btn_search->setToolTip(tr("Push to start search."));
  layout->addWidget(btn_search, _r++, 2, 1, 1, Qt::AlignLeft);

  m_tableWidget = new QTableWidget(this);
  m_tableWidget->setColumnCount(5);
  m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tableWidget->setHorizontalHeaderItem(
      0, new QTableWidgetItem(tr("Id"), QTableWidgetItem::UserType));
  m_tableWidget->setHorizontalHeaderItem(
      1, new QTableWidgetItem(tr("Organistaion"), QTableWidgetItem::UserType));
  m_tableWidget->setHorizontalHeaderItem(
      2, new QTableWidgetItem(tr("Fullname"), QTableWidgetItem::UserType));
  m_tableWidget->setHorizontalHeaderItem(
      3, new QTableWidgetItem(tr("Imported name"), QTableWidgetItem::UserType));
  m_tableWidget->setHorizontalHeaderItem(
      4, new QTableWidgetItem(tr("Address info"), QTableWidgetItem::UserType));
  layout->addWidget(m_tableWidget, _r++, 0, 1, 3);

  QHeaderView* m_view = m_tableWidget->horizontalHeader();
  m_view->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_view->setStretchLastSection(true);

  QPushButton* btn_next = new QPushButton(tr("Further"), this);
  btn_next->setIcon(AntiquaCRM::antiquaIcon("go-next-view"));
  btn_next->setToolTip(tr("Go next view in change data."));
  btn_next->setEnabled(false);
  layout->addWidget(btn_next, _r++, 2, 1, 1);

  setLayout(layout);

  // signals
  connect(btn_search, SIGNAL(clicked()), SLOT(searchClicked()));
  connect(m_searchLine, SIGNAL(editingFinished()), SLOT(searchClicked()));
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
  QString _field;
  QString _search = m_searchLine->text().trimmed().toLower();
  if (_search.length() < 1)
    return;

  QVariant _selected = m_comboBox->itemData(m_comboBox->currentIndex());
  if (_selected.metaType().id() == QMetaType::QStringList) {
    _field = _selected.toStringList().join("%").toLower();
  } else {
    _field = _selected.toString().trimmed().toLower();
  }

  QString _sql("SELECT c_id, c_company_name, ");
  _sql.append("CONCAT(c_firstname, ' ', c_lastname) AS name,");
  _sql.append("c_provider_import AS import,");
  _sql.append("CONCAT(c_postalcode, ' ', c_location, ' ', c_street) AS address");
  _sql.append(" FROM customers WHERE ");
  _sql.append(_field);
  if (_field.contains("c_id")) {
    static const QRegularExpression _pattern("\\D");
    _search = _search.replace(_pattern, "");
    if (_search.length() < 1)
      return;

    _sql.append("=");
    _sql.append(_search);
  } else {
    QStringList _list = _search.split(" ");
    _sql.append(" ILIKE '%");
    _sql.append(_list.join("%"));
    _sql.append("%'");
  }
  _sql.append(" ORDER BY c_id LIMIT 99;");

  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql != nullptr) {
    QSqlQuery _q = m_sql->query(_sql);
    if (_q.size() < 1) {
#ifdef ANTIQUA_DEVELOPMENT
      qDebug() << Q_FUNC_INFO << _sql;
#endif
      emit sendNotification(tr("Nothing found."));
      return;
    }

    // start output
    qint32 _row = 0;
    m_tableWidget->clearContents();
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
