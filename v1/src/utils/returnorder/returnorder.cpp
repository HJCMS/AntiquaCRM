// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "returnorder.h"
#include "returnedit.h"
#include "returninfowidget.h"
#include "returnnavigation.h"

#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QPushButton>

ReturnOrder::ReturnOrder(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(600, 300);
  setWindowTitle(tr("Dialog to create a return!") + "[*]");
  setWindowIcon(QIcon("://icons/action_undo.png"));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 0, 5, 0);

  m_stackedWidget = new QStackedWidget(this);
  m_info = new ReturnInfoWidget(m_stackedWidget);
  m_stackedWidget->insertWidget(0, m_info);

  m_medit = new ReturnEdit(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_medit);
  layout->addWidget(m_stackedWidget);

  m_btnBox = new ReturnNavigation(this);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_info, SIGNAL(sendConfirm()), SLOT(setStep1()));
  connect(m_medit, SIGNAL(sendPaymentIds(const QStringList &)),
          SLOT(setStep2(const QStringList &)));

  connect(m_btnBox, SIGNAL(sendSave()), SLOT(setFinal()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
  connect(m_medit, SIGNAL(sendMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

bool ReturnOrder::initData(qint64 orderId) {
  AntiquaCRM::ASqlFiles tpl("query_order_by_oid");
  if (!tpl.openTemplate())
    return false;

  tpl.setWhereClause("o_id=" + QString::number(orderId));
  m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(tpl.getQueryContent());
  if (q.size() < 1)
    return false;

  QSqlRecord r = q.record();
  while (q.next()) {
    for (int i = 0; i < r.count(); i++) {
      QSqlField f = r.field(i);
      InputEdit *e = m_medit->findChild<InputEdit *>(f.name());
      if (e != nullptr)
        e->setValue(q.value(f.name()));
    }
  }

  QString sql("SELECT * FROM article_orders WHERE a_order_id=");
  sql.append(QString::number(orderId) + ";");
  q = m_sql->query(sql);
  if (q.size() > 0) {
    m_medit->loadArticleData(q);
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
  return true;
}

void ReturnOrder::setStep1() {
  // Article selection
  m_stackedWidget->setCurrentIndex(1);
}

void ReturnOrder::setStep2(const QStringList &ids) {
  if (ids.size() < 1)
    return;

  qint64 o_id = m_medit->o_id->value().toInt();
  if (o_id < 1)
    return;

  AntiquaCRM::ASqlFiles tpl("query_returning_articles");
  if (!tpl.openTemplate())
    return;

  QStringList sqlUpdates;
  QString o_order_status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
  QString o_payment_status = QString::number(AntiquaCRM::OrderPayment::RETURN);
  QString upd_order("UPDATE inventory_orders SET ");
  upd_order.append("o_order_status=" + o_order_status);
  upd_order.append(", o_payment_status=" + o_payment_status);
  upd_order.append(", o_modified=CURRENT_TIMESTAMP");
  upd_order.append(", o_delivered=CURRENT_TIMESTAMP");
  upd_order.append(" WHERE o_id=" + QString::number(o_id) + ";");
  sqlUpdates << upd_order;

  QString sql("a_order_id=");
  sql.append(QString::number(o_id));
  sql.append(" AND a_payment_id IN (" + ids.join(",") + ")");
  tpl.setWhereClause(sql);

  QSqlQuery q = m_sql->query(tpl.getQueryContent());
  if (q.size() > 0) {
    while (q.next()) {
      qint64 a_payment_id = q.value("a_payment_id").toInt();
      if (a_payment_id < 1)
        continue;

      double price = q.value("a_sell_price").toDouble();
      QString upd_article("UPDATE article_orders SET a_sell_price=");
      if (price > 0)
        upd_article.append("-");

      upd_article.append(QString::number(price, 'f', 2));
      upd_article.append(",a_modified=CURRENT_TIMESTAMP");
      upd_article.append(" WHERE a_payment_id=");
      upd_article.append(QString::number(a_payment_id) + ";");
      sqlUpdates << upd_article;
    }
  }

  m_sql->query(sqlUpdates.join("\n"));
  if (m_sql->lastError().isEmpty()) {
    accept();
  } else {
    m_statusBar->showMessage(tr("An error has occurred!"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
  }
}

void ReturnOrder::setFinal() {
  qDebug() << Q_FUNC_INFO << "TODO";
  accept();
}

int ReturnOrder::exec() {
  qWarning("Don't use this Dialog without OrderID!");
  return QDialog::Rejected;
}

int ReturnOrder::exec(qint64 orderId) {
  if (orderId < 1)
    return QDialog::Rejected;

  if (!initData(orderId))
    return QDialog::Rejected;

  return QDialog::exec();
}
