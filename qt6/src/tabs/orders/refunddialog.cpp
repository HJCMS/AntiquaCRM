// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refunddialog.h"

#include <QJsonDocument>

RefundingDialog::RefundingDialog(qint64 orderId, QWidget *parent)
    : AntiquaCRM::ADialog{parent}, p_orderid{orderId} {
  setWindowTitle(tr("Refunding") + " [*]");
  setMinimumSize(700, 520);

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("refund_navi");
  // BEGIN:Pages
  m_intro = new RefundIntroduction(m_stackedWidget);
  m_stackedWidget->insertWidget(0, m_intro);

  m_select = new RefundSelecter(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_select);

  m_final = new RefundFinalize(m_stackedWidget);
  m_stackedWidget->insertWidget(2, m_final);
  // END:Pages

  layout->addWidget(m_stackedWidget);
  layout->setStretch(0, 1);

  // NavigationBar
  m_navBar = new AntiquaCRM::ANavigationBar(this, m_stackedWidget->count());
  layout->addWidget(m_navBar);

  // ButtonsBar
  btn_reject->setText(tr("Close"));
  btn_reject->setIcon(AntiquaCRM::antiquaIcon("action-quit"));

  const QString ptip(tr("Starts refunding and closes this dialog on success!"));
  btn_apply->setToolTip(ptip);
  btn_apply->setStatusTip(btn_apply->toolTip());

  // Signals
  connect(m_navBar, SIGNAL(sendNext()), SLOT(goForward()));
  connect(m_intro, SIGNAL(sendForward()), SLOT(goForward()));
  connect(m_navBar, SIGNAL(sendPrev()), SLOT(goBackward()));
  connect(m_intro, SIGNAL(sendBackward()), SLOT(goBackward()));
  connect(m_stackedWidget, SIGNAL(currentChanged(int)), SLOT(pageChanged(int)));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
  connect(m_select, SIGNAL(sendStatus(bool)), SLOT(statusFromPage(bool)));
  connect(m_final, SIGNAL(sendStatus(bool)), SLOT(statusFromPage(bool)));
  // Apply button handling
  connect(m_final, SIGNAL(sendEnableButton(bool)), btn_apply,
          SLOT(setEnabled(bool)));
  connect(btn_apply, SIGNAL(clicked()), SLOT(saveAndQuit()));
}

void RefundingDialog::goForward() {
  int _i = (m_stackedWidget->currentIndex() + 1);
  if (_i == m_stackedWidget->indexOf(m_final)) {
    m_final->addArticles(m_select->selectedRefunds());
  }
  m_stackedWidget->setCurrentIndex(_i);
}

void RefundingDialog::goBackward() {
  int _i = (m_stackedWidget->currentIndex() - 1);
  m_stackedWidget->setCurrentIndex(_i);
}

void RefundingDialog::pageChanged(int index) {
  m_navBar->setAllowPrev(index);
  m_navBar->setCurrentIndex(index);
}

void RefundingDialog::statusFromPage(bool status) {
  RefundingPage *m_w = qobject_cast<RefundingPage *>(sender());
  if (m_w == nullptr)
    return;

  int _index = m_stackedWidget->indexOf(m_w);
  if (_index > 0)
    m_navBar->setAllowNext(_index, status);
}

void RefundingDialog::saveAndQuit() {
  QMap<qint64, double> _costs = m_final->getFinalRefunding();
  if (_costs.size() < 1 || p_orderid < 1) {
    m_statusBar->showMessage(tr("Nothing todo."), 5000);
    return;
  }

  // Order Id
  const QString _oid = QString::number(p_orderid);

  // Begin:Update
  QStringList _upd;
  QString _payment_status = QString::number(AntiquaCRM::OrderPayment::RETURN);
  QString _order("UPDATE inventory_orders SET ");
  _order.append("o_payment_status=" + _payment_status);
  _order.append(", o_modified=CURRENT_TIMESTAMP");
  _order.append(" WHERE o_id=" + _oid + ";");
  _upd << _order;

  QStringList _payment_ids;
  QListIterator<qint64> _keys(_costs.keys());
  while (_keys.hasNext()) {
    qint64 _i = _keys.next();
    if (_i > 0)
      _payment_ids << QString::number(_i);
  }

  AntiquaCRM::ASqlFiles _tpl("query_returning_articles");
  if (!_tpl.openTemplate()) {
    m_statusBar->showMessage(tr("Database template error!"), 8000);
    return;
  }

  QString _sql("a_order_id=" + _oid);
  _sql.append(" AND a_payment_id IN (" + _payment_ids.join(",") + ")");
  _tpl.setWhereClause(_sql);

  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() > 0) {
    while (_q.next()) {
      qint64 p_id = _q.value("a_payment_id").toLongLong();
      QString _rc = QString::number(_costs.value(p_id), 'f', 2);
      if (p_id > 0 && _rc.length() > 0) {
        const QString _id = QString::number(p_id);
        _upd << QString("UPDATE article_orders SET "
                        "a_sell_price=CONCAT('-', a_sell_price)::NUMERIC,"
                        "a_modified=CURRENT_TIMESTAMP,"
                        "o_delivered=CURRENT_TIMESTAMP,"
                        "a_refunds_cost=%1 WHERE a_payment_id=%2;")
                    .arg(_rc, _id);
      }
    }
  }

  if (_upd.size() < 2) {
    m_statusBar->showMessage(tr("Nothing todo."), 5000);
    return;
  }

  // Create final update statement
  const QString _sql_update(_upd.join("\n"));
  m_sql->query(_sql_update);
  if (m_sql->lastError().isEmpty()) {
    qDebug() << "SUCCSESS:" << _sql_update;
    accept();
  } else {
    m_statusBar->showMessage(tr("An error has occurred!"), 10000);
  }
}

int RefundingDialog::exec() {
  // Init PgSQL and call settings
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!m_sql->status()) {
    qWarning("Missing database connection!");
    return QDialog::Rejected;
  }

  QJsonObject _conf;
  _conf.insert("default", QJsonValue(1.00));

  AntiquaCRM::ASqlFiles _tpl("query_antiquacrm_config");
  if (!_tpl.openTemplate())
    return false;

  _tpl.setWhereClause("cfg_group='CONFIG_REFUNDING';");
  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() == 1) {
    _q.next();
    _conf = QJsonDocument::fromJson(_q.value(0).toByteArray()).object();
    if (_conf.isEmpty()) {
      m_statusBar->showMessage(tr("No configuration found!"), 10000);
    }
  }
  _q.clear();

  if (p_orderid < 1) {
    qWarning("Missing Order Number!");
    return QDialog::Rejected;
  }

  // init Page data
  for (int i = 0; i < m_stackedWidget->count(); i++) {
    RefundingPage *w =
        qobject_cast<RefundingPage *>(m_stackedWidget->widget(i));
    if (w == nullptr)
      continue;

    w->setRefundingConfig(_conf);
    w->initPageData(m_sql, p_orderid);
  }

  return AntiquaCRM::ADialog::exec();
}
