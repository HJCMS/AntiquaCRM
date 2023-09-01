// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provideractiondialog.h"

namespace AntiquaCRM {

ProviderActionDialog::ProviderActionDialog(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(500, 400);
  setContentsMargins(0, 0, 0, 0);

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->setContentsMargins(5, 5, 5, 5);

  m_scrollArea = new QScrollArea(this);
  m_scrollArea->setFrameStyle(QFrame::NoFrame);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_scrollArea);

  m_buttonBox = new QDialogButtonBox(this);
  m_buttonBox->addButton(QDialogButtonBox::Apply);
  m_buttonBox->addButton(QDialogButtonBox::Ok);
  m_buttonBox->addButton(QDialogButtonBox::Cancel);
  m_buttonBox->addButton(QDialogButtonBox::Close);
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_buttonBox, SIGNAL(clicked(QAbstractButton *)),
          SLOT(buttonAction(QAbstractButton *)));
}

void ProviderActionDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    if (isWindowModified()) {
      e->setAccepted(false);
      m_statusBar->showMessage(tr("There are changes in this Dialog!"));
    }
  }
  QDialog::closeEvent(e);
}

void ProviderActionDialog::buttonAction(QAbstractButton *button) {
  switch (m_buttonBox->standardButton(button)) {
  case (QDialogButtonBox::Apply): {
    emit sendApplyClicked();
  } break;

  case (QDialogButtonBox::Ok): {
    emit sendOkClicked();
  } break;

  case (QDialogButtonBox::Cancel): {
    emit sendCancelClicked();
  } break;

  case (QDialogButtonBox::Close): {
    emit sendCloseClicked();
  } break;

  default:
    qWarning("This Action '%s' is not registered", qPrintable(button->text()));
    break;
  }
}

void ProviderActionDialog::statusMessage(const QString &message) {
  m_statusBar->showMessage(message);
}

int ProviderActionDialog::exec() {
  // todo something here
  return QDialog::exec();
}

} // namespace AntiquaCRM
