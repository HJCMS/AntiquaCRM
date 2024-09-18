// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellerimportrepair.h"
#include "importrepairedit.h"
#include "importrepairfinder.h"

#include <QIcon>
#include <QJsonDocument>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSqlRecord>

SellerImportRepair::SellerImportRepair(const QString& provider, const QString& order,
                                       QWidget* parent)
    : AntiquaCRM::ADialog{parent}, p_provider{provider}, p_orderid{order} {
  setObjectName("seller_import_repair_dialog");

  QLabel* info = new QLabel(this);
  info->setText(
      tr("You can use this input mask to repair customer data "
         "imports from your service provider."));
  layout->addWidget(info);

  m_mainWidget = new QStackedWidget(this);

  m_finder = new ImportRepairFinder(this);
  m_mainWidget->insertWidget(0, m_finder);

  m_cedit = new ImportRepairEdit(this);
  m_mainWidget->insertWidget(1, m_cedit);

  layout->addWidget(m_mainWidget);
  layout->setStretch(1, 1);

  m_navbar = new AntiquaCRM::ANavigationBar(this);
  layout->addWidget(m_navbar);

  btn_clear = m_buttonsBar->addButton(QDialogButtonBox::Reset);
  btn_clear->setToolTip(tr("Clear search input"));
  btn_clear->setStatusTip(btn_clear->toolTip());

  btn_apply->setToolTip(tr("Save current changes"));
  btn_apply->setStatusTip(btn_apply->toolTip());

  // ImportsNavBar
  connect(m_navbar, SIGNAL(sendPrev()), SLOT(setStartPage()));
  connect(m_navbar, SIGNAL(sendNext()), SLOT(setEditPage()));

  // ButtonBox
  connect(btn_clear, SIGNAL(clicked()), m_finder, SLOT(clear()));
  connect(btn_apply, SIGNAL(clicked()), SLOT(updateData()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));

  // Search
  connect(m_cedit, SIGNAL(sendFindClause(QString)), SLOT(findSystemCustomer(QString)));
  connect(m_finder, SIGNAL(sendFindClause(QString)), SLOT(findSystemCustomer(QString)));
  connect(m_finder, SIGNAL(sendUseClause(qint64)), SLOT(openSystemCustomer(qint64)));
}

bool SellerImportRepair::init() {
  if (m_sql == nullptr) // first tray
    m_sql = new AntiquaCRM::ASqlCore(this);

  QString _sql("SELECT pr_order_data FROM provider_orders");
  _sql.append(" WHERE (pr_order='" + p_orderid + "'");
  _sql.append(" AND pr_name ILIKE '" + p_provider + "');");

  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() != 1) {
    qWarning("Missing data for OrderId:(%s).", qPrintable(p_orderid));
    return false;
  }
  _q.next();

  QByteArray _buffer = _q.value("pr_order_data").toByteArray();
  p_json = QJsonDocument::fromJson(_buffer).object();
  if (p_json.isEmpty()) {
    qWarning("Missing Json from pr_order_data");
    return false;
  }

  _sql.clear();
  _buffer.clear();
  _q.clear();

  m_cedit->setImportData(p_json.value("customer").toObject());
  return true;
}

void SellerImportRepair::setStartPage() {
  m_mainWidget->setCurrentIndex(0);
  m_navbar->setAllowPrev(0);
  m_navbar->setAllowNext(0, false);
  btn_apply->setEnabled(false);
}

void SellerImportRepair::setEditPage() {
  m_mainWidget->setCurrentIndex(1);
  m_navbar->setAllowPrev(1);
  m_navbar->setAllowNext(1, false);
  btn_apply->setEnabled(true);
}

void SellerImportRepair::updateData() {
  QJsonObject _json(p_json);
  _json.remove("customer");
  _json.insert("customer", m_cedit->getData());

  QJsonDocument _doc(_json);
  QString _sql("UPDATE provider_orders SET pr_customer_id=");
  _sql.append(QString::number(m_cedit->getCustomerId()));
  _sql.append(",pr_order_data='");
  _sql.append(_doc.toJson(QJsonDocument::Compact));
  _sql.append("' WHERE (pr_order='" + p_orderid + "'");
  _sql.append(" AND pr_name ILIKE '" + p_provider + "');");
  m_sql->query(_sql);

  if (m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("Success"));
    m_finder->clear();
    setWindowModified(false);
    setStartPage();
    init();
  } else {
    m_statusBar->showMessage(tr("Failed"));
    btn_apply->setEnabled(true);
    setWindowModified(true);
  }
}

void SellerImportRepair::findSystemCustomer(const QString& clause) {
  bool _merge_call = (clause.contains("c_id", Qt::CaseSensitive));
  QString _sql("SELECT * FROM customers WHERE " + clause + ";");
  // qDebug() << Q_FUNC_INFO << _sql;
  QSqlQuery _q = m_sql->query(_sql);
  if (m_sql->lastError().isEmpty()) {
    if (_q.size() > 0) {
      QSqlRecord _r = _q.record();
      while (_q.next()) {
        QJsonObject _item;
        for (int c = 0; c < _r.count(); c++) {
          QSqlField _field = _r.field(c);
          _item.insert(_field.name(), _q.value(_field.name()).toJsonValue());
        }
        if (_merge_call) {
          // only one entry called by c_id
          m_cedit->setOriginData(_item);
          // enable it
          m_navbar->setAllowNext(0, true);
        } else {
          m_finder->addCustomer(_item);
        }
      }
    }
    setWindowModified(false);
  } else {
    m_statusBar->showMessage(tr("Failed"));
    setWindowModified(true);
  }
}

void SellerImportRepair::openSystemCustomer(qint64 cid) {
  if (cid < 1)
    return;

  // rgister CustomerId to ImportRepairEdit
  m_cedit->setCustomerId(cid);

  QString _str("c_id=");
  _str.append(QString::number(cid));
  findSystemCustomer(_str);
}

int SellerImportRepair::exec() {
  if (p_orderid.isEmpty() || p_provider.isEmpty())
    return QDialog::Rejected;

  QString _info = tr("Edit %1 order %2").arg(p_provider, p_orderid);
  setWindowTitle(_info + " [*]");

  if (!init())
    return QDialog::Rejected;

  return QDialog::exec();
}
