// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provideractiondialog.h"
#include "provideractionnavigator.h"

#include <AntiquaWidgets>
#include <QBoxLayout>
#include <QPushButton>
#include <QStatusTipEvent>

namespace AntiquaCRM {

ProviderActionDialog::ProviderActionDialog(QWidget *parent, bool remote)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(500, 400);
  setContentsMargins(0, 0, 0, 0);
  setWindowTitle(tr("Provider Actions Dialog") + " [*]");
  setWindowIcon(AntiquaCRM::antiquaIcon("view-financial-transfer"));

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->setContentsMargins(5, 5, 5, 5);

  QScrollArea *m_scrollArea = new QScrollArea(this);
  m_scrollArea->setFrameStyle(QFrame::NoFrame);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_scrollArea);

  stackedWidget = new QStackedWidget(m_scrollArea);
  stackedWidget->setObjectName("central_widget");
  stackedWidget->setContentsMargins(0, 0, 0, 0);
  m_scrollArea->setWidget(stackedWidget);

  m_navigator = new ProviderActionNavigator(stackedWidget, this);
  m_navigator->setEnabled(false);
  m_navigator->setVisible(false);
  layout->addWidget(m_navigator);

  m_buttonBox = new QDialogButtonBox(this);
  if (remote) {
    QPushButton *btn_send = m_buttonBox->addButton(QDialogButtonBox::Apply);
    btn_send->setText(tr("Submit"));
    btn_send->setToolTip(tr("Sends selected changes to your provider."));
    btn_send->setIcon(windowIcon());
  }

  QPushButton *btn_cancel = m_buttonBox->addButton(QDialogButtonBox::Cancel);
  btn_cancel->setToolTip(tr("Cancel all operations and close this Dialog."));

  m_buttonBox->addButton(QDialogButtonBox::Close);
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_navigator, SIGNAL(sendGotoPage(int)), stackedWidget,
          SLOT(setCurrentIndex(int)));

  connect(m_buttonBox, SIGNAL(clicked(QAbstractButton *)),
          SLOT(buttonAction(QAbstractButton *)));
}

void ProviderActionDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    if (isWindowModified()) {
      e->setAccepted(false);
      setRejectMessage();
      return;
    }
  }
  QDialog::closeEvent(e);
}

bool ProviderActionDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip && e->isAccepted()) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t == nullptr)
      return false;

    m_statusBar->showMessage(t->tip(), 5000);
    return true;
  }
  return QDialog::event(e);
}

void ProviderActionDialog::setRejectMessage() {
  const QString _m(tr("This Dialog have changes, close request rejected!"));
  m_statusBar->showMessage(_m, 5000);
}

void ProviderActionDialog::buttonAction(QAbstractButton *button) {
  switch (m_buttonBox->standardButton(button)) {
  case (QDialogButtonBox::Apply):
    emit sendSubmitClicked();
    break;

  case (QDialogButtonBox::Cancel):
    done(QDialog::Rejected);
    break;

  case (QDialogButtonBox::Close): {
    if (isWindowModified()) {
      setRejectMessage();
      return;
    }
    done(QDialog::Accepted);
  } break;

  default:
    qWarning("Unregistered action '%s'!", qPrintable(button->text()));
    break;
  }
}

void ProviderActionDialog::statusMessage(const QString &message) {
  m_statusBar->showMessage(message);
}

int ProviderActionDialog::exec() {
  if (stackedWidget->count() > 1) {
    m_navigator->setEnabled(true);
    m_navigator->setVisible(true);
  }
  setWindowModified(false);
  return QDialog::exec();
}

void ProviderActionDialog::isChildWindowModified(bool b) {
  setWindowModified(b);
}

} // namespace AntiquaCRM
