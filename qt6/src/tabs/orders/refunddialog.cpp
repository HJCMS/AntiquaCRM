// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refunddialog.h"

RefundingDialog::RefundingDialog(qint64 orderId, QWidget *parent)
    : AntiquaCRM::ADialog{parent}, p_orderid{orderId} {
  setWindowTitle(tr("Refunding") + " [*]");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("refund_navi");
  // BEGIN:Pages
  m_intro = new RefundIntroduction(m_stackedWidget);
  m_stackedWidget->insertWidget(0, m_intro);

  m_page1 = new RefundStepOne(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_page1);

  m_page2 = new RefundStepTwo(m_stackedWidget);
  m_stackedWidget->insertWidget(2, m_page2);

  // END:Pages
  layout->addWidget(m_stackedWidget);
  layout->setStretch(0, 1);

  m_navBar = new AntiquaCRM::ANavigationBar(this, m_stackedWidget->count());
  layout->addWidget(m_navBar);

  // ButtonsBar
  btn_reject->setText(tr("Close"));
  btn_reject->setIcon(AntiquaCRM::antiquaIcon("action-quit"));

  btn_commit = m_buttonsBar->addButton(QDialogButtonBox::Apply);
  btn_commit->setIcon(AntiquaCRM::antiquaIcon("action-save"));

  // Signals
  connect(m_navBar, SIGNAL(sendNext()), SLOT(goForward()));
  connect(m_intro, SIGNAL(sendForward()), SLOT(goForward()));
  connect(m_navBar, SIGNAL(sendPrev()), SLOT(goBackward()));
  connect(m_intro, SIGNAL(sendBackward()), SLOT(goBackward()));
  connect(m_stackedWidget, SIGNAL(currentChanged(int)), m_navBar,
          SLOT(setIndex(int)));
  connect(btn_commit, SIGNAL(clicked()), SLOT(commit()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
}

void RefundingDialog::goForward() {
  qint8 _i = (m_stackedWidget->currentIndex() + 1);
  m_stackedWidget->setCurrentIndex(_i);
}

void RefundingDialog::goBackward() {
  qint8 _i = (m_stackedWidget->currentIndex() - 1);
  m_stackedWidget->setCurrentIndex(_i);
}

void RefundingDialog::commit() {
  qDebug() << Q_FUNC_INFO << "__TODO__";
  if (false) // TODO
    m_statusBar->showMessage(tr("Update Success"), 5000);
  else
    m_statusBar->showMessage(tr("Update failed!"), 10000);
}

int RefundingDialog::exec() {
  if (!m_page1->addDataRecord(p_orderid))
    return QDialog::Rejected;

  return AntiquaCRM::ADialog::exec();
}
