// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "importsedit.h"
#include "importcustomeredit.h"
#include "importsfindexisting.h"
#include "importsnavbar.h"

#include <QIcon>
#include <QJsonDocument>
#include <QLayout>
#include <QPushButton>
#include <QSqlRecord>

ImportsEdit::ImportsEdit(const QString &provider, const QString &order,
                         QWidget *parent)
    : QDialog{parent}, p_provider{provider}, p_orderid{order} {
  setObjectName("imports_edit_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(550, 450);
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 0, 5, 0);

  QLabel *info = new QLabel(this);
  info->setText(tr("You can use this input mask to repair customer data "
                   "imports from your service provider."));
  layout->addWidget(info);

  m_mainWidget = new QStackedWidget(this);

  m_finder = new ImportsFindExisting(this);
  m_mainWidget->insertWidget(0, m_finder);

  m_cedit = new ImportCustomerEdit(this);
  m_mainWidget->insertWidget(1, m_cedit);

  layout->addWidget(m_mainWidget);
  layout->setStretch(1, 1);

  m_navbar = new ImportsNavBar(this);
  layout->addWidget(m_navbar);

  m_buttonsBar = new QDialogButtonBox(Qt::Horizontal, this);
  QPushButton *btn_clear = m_buttonsBar->addButton(QDialogButtonBox::Reset);
  btn_clear->setToolTip(tr("Clear search input"));
  btn_clear->setStatusTip(btn_clear->toolTip());

  QPushButton *btn_save = m_buttonsBar->addButton(QDialogButtonBox::Save);
  btn_save->setToolTip(tr("Save current changes"));
  btn_save->setStatusTip(btn_save->toolTip());

  QPushButton *btn_close = m_buttonsBar->addButton(QDialogButtonBox::Close);
  btn_close->setToolTip(tr("Close this editor"));
  btn_close->setStatusTip(btn_close->toolTip());

#ifdef Q_OS_WIN
  btn_clear->setIcon(QIcon(":/icons/action_reload.png"));
  btn_save->setIcon(QIcon(":/icons/action_save.png"));
  btn_close->setIcon(QIcon(":/icons/action_quit.png"));
#endif

  layout->addWidget(m_buttonsBar);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  // ImportsNavBar
  connect(m_navbar, SIGNAL(sendPrevPage()), SLOT(setStartPage()));
  connect(m_navbar, SIGNAL(sendNextPage()), SLOT(setEditPage()));
  // ButtonBox
  connect(btn_clear, SIGNAL(clicked()), m_finder, SLOT(clear()));
  connect(btn_save, SIGNAL(clicked()), SLOT(updateData()));
  connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
  // Search
  connect(m_cedit, SIGNAL(sendFindClause(const QString &)),
          SLOT(findSystemCustomer(const QString &)));
  connect(m_finder, SIGNAL(sendFindClause(const QString &)),
          SLOT(findSystemCustomer(const QString &)));
  connect(m_finder, SIGNAL(sendUseClause(const QString &)),
          SLOT(findSystemCustomer(const QString &)));
}

void ImportsEdit::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    // Key press enter or return must ignored!
    return;
  }
  QDialog::keyPressEvent(e);
}

bool ImportsEdit::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void ImportsEdit::setStartPage() {
  m_mainWidget->setCurrentIndex(0);
  m_navbar->setCurrentIndex(0);
}

void ImportsEdit::setEditPage() {
  m_mainWidget->setCurrentIndex(1);
  m_navbar->setCurrentIndex(1);
}

void ImportsEdit::updateData() {
  QJsonObject _json(p_json);
  _json.remove("customer");
  _json.insert("customer", m_cedit->getData());

  QJsonDocument _doc(_json);
  QString _sql("UPDATE provider_orders SET pr_order_data='");
  _sql.append(_doc.toJson(QJsonDocument::Compact));
  _sql.append("' WHERE (pr_order='" + p_orderid + "'");
  _sql.append(" AND pr_name ILIKE '" + p_provider + "');");
  m_sql->query(_sql);
  if (m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("Success"));
    setWindowModified(false);
  } else {
    m_statusBar->showMessage(tr("Failed"));
    setWindowModified(true);
  }
}

void ImportsEdit::findSystemCustomer(const QString &clause) {
  bool _merge_call = (clause.contains("c_id", Qt::CaseSensitive));
  QString _sql("SELECT * FROM customers WHERE " + clause + ";");
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
          m_navbar->setCurrentIndex(0);
        } else {
          m_finder->addCustomer(_item);
        }
      }
    }
  } else {
    qDebug() << Q_FUNC_INFO << _sql;
    m_statusBar->showMessage(tr("Failed"));
    setWindowModified(true);
  }
}

int ImportsEdit::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr || p_orderid.isEmpty() || p_provider.isEmpty())
    return QDialog::Rejected;

  QString _info = tr("Edit %1 order %2").arg(p_provider, p_orderid);
  setWindowTitle(_info + " [*]");

  QString _sql("SELECT pr_order_data FROM provider_orders");
  _sql.append(" WHERE (pr_order='" + p_orderid + "'");
  _sql.append(" AND pr_name ILIKE '" + p_provider + "');");
  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() != 1) {
    qWarning("Missing data for OrderId:(%s).", qPrintable(p_orderid));
    return QDialog::Rejected;
  }
  _q.next();
  QByteArray _buffer = _q.value("pr_order_data").toByteArray();
  p_json = QJsonDocument::fromJson(_buffer).object();
  if (p_json.isEmpty()) {
    qWarning("Missing Json from pr_order_data");
    return QDialog::Rejected;
  }

  _sql.clear();
  _buffer.clear();
  _q.clear();

  m_cedit->setImportData(p_json.value("customer").toObject());
  return QDialog::exec();
}
