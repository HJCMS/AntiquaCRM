// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refunddialog.h"

RefundingDialog::RefundingDialog(qint64 orderId, QWidget *parent)
    : AntiquaCRM::ADialog{parent}, p_orderid{orderId} {
  setWindowTitle(tr("Refunding") + " [*]");

  m_stackedWidget = new QStackedWidget(this);
  layout->addWidget(m_stackedWidget);
  layout->setStretch(0, 1);

  m_navBar = new AntiquaCRM::ANavigationBar(this);
  layout->addWidget(m_navBar);

  // ButtonsBar
  btn_reject->setText(tr("Close"));
  btn_reject->setIcon(AntiquaCRM::antiquaIcon("action-quit"));

  btn_commit = m_buttonsBar->addButton(QDialogButtonBox::Apply);
  btn_commit->setIcon(AntiquaCRM::antiquaIcon("action-save"));

  // Signals
  connect(btn_commit, SIGNAL(clicked()), SLOT(commit()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
}

void RefundingDialog::commit() {
  if (false) // TODO
    m_statusBar->showMessage(tr("Update Success"), 5000);
  else
    m_statusBar->showMessage(tr("Update failed!"), 10000);
}

int RefundingDialog::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  //
  //
  return AntiquaCRM::ADialog::exec();
}
