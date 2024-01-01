// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "adialog.h"

#include <QMessageBox>
#include <QStatusTipEvent>

namespace AntiquaCRM {

ADialog::ADialog(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(450, 450);
  setContentsMargins(5, 2, 5, 0);

  QVBoxLayout *mLayout = new QVBoxLayout(this);
  mLayout->setContentsMargins(0, 0, 0, 0);

  layout = new QBoxLayout(QBoxLayout::TopToBottom);
  mLayout->addLayout(layout);

  m_buttonsBar = new QDialogButtonBox(this);
  btn_reject = m_buttonsBar->addButton(QDialogButtonBox::Cancel);
  btn_reject->setToolTip(tr("Close without saving"));
  btn_reject->setStatusTip(btn_reject->toolTip());

  btn_apply = m_buttonsBar->addButton(QDialogButtonBox::Apply);
  btn_apply->setToolTip(tr("Apply current changes"));
  btn_apply->setStatusTip(btn_apply->toolTip());
  btn_apply->setEnabled(false);

  mLayout->addWidget(m_buttonsBar);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  mLayout->addWidget(m_statusBar);

  setLayout(mLayout);
}

void ADialog::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    return;

  QDialog::keyPressEvent(event);
}

bool ADialog::event(QEvent *event) {
  if (event->type() == QEvent::StatusTip) {
    QStatusTipEvent *m_t = static_cast<QStatusTipEvent *>(event);
    if (m_t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(m_t->tip(), 1000);
    return true;
  }
  return QDialog::event(event);
}

void ADialog::closeEvent(QCloseEvent *event) {
  if (event->type() == QEvent::Close) {
    if (isWindowModified()) {
      event->setAccepted(false);
      QString info = tr("You have unsaved changes!");
      info.append("<p>" + tr("Do you really want to close?") + "</p>");
      int ret = QMessageBox::question(this, tr("Unsaved Changes!"), info);
      if (ret == QMessageBox::No)
        return;
    }
  }
  QDialog::closeEvent(event);
}

} // namespace AntiquaCRM
