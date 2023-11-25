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
  btn_commit = m_buttonsBar->addButton(QDialogButtonBox::Apply);
  btn_commit->setIcon(AntiquaCRM::antiquaIcon("action-save"));
  btn_commit->setEnabled(false);

  // Signals
  connect(m_navBar, SIGNAL(sendNext()), SLOT(goForward()));
  connect(m_intro, SIGNAL(sendForward()), SLOT(goForward()));
  connect(m_navBar, SIGNAL(sendPrev()), SLOT(goBackward()));
  connect(m_intro, SIGNAL(sendBackward()), SLOT(goBackward()));
  connect(m_stackedWidget, SIGNAL(currentChanged(int)), SLOT(pageChanged(int)));
  connect(btn_commit, SIGNAL(clicked()), SLOT(commit()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
  connect(m_select, SIGNAL(sendStatus(bool)), SLOT(statusFromPage(bool)));
  connect(m_final, SIGNAL(sendStatus(bool)), SLOT(statusFromPage(bool)));
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

void RefundingDialog::commit() {
  qDebug() << Q_FUNC_INFO << "__TODO__ FINAL COMMIT";
  if (false)
    m_statusBar->showMessage(tr("Update Success"), 5000);
  else
    m_statusBar->showMessage(tr("Update failed!"), 10000);
}

int RefundingDialog::exec() {
  // Init PgSQL and call settings
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!m_sql->status()) {
    qWarning("Missing database connection!");
    return QDialog::Rejected;
  }

  QJsonObject _conf;
  _conf.insert("default", QJsonValue(1.25));

  const QString _sql("SELECT DISTINCT cfg_jsconfig FROM antiquacrm_configs "
                     "WHERE cfg_group='CONFIG_REFUNDING';");

  QSqlQuery _q = m_sql->query(_sql);
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
