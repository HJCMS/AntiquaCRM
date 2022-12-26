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
  setMinimumSize(450, 300);
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

  connect(m_info, SIGNAL(sendConfirm()), SLOT(setEditPage()));
  connect(m_btnBox, SIGNAL(sendSave()), SLOT(save()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
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
      else
        qDebug() << f.name() << q.value(f.name());
    }
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
  return true;
}

void ReturnOrder::setEditPage() { m_stackedWidget->setCurrentIndex(1); }

void ReturnOrder::save() { qDebug() << Q_FUNC_INFO << "TODO"; }

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
