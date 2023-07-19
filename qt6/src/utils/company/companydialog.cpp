// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "companydialog.h"
#include "companywidget.h"

#include <QIcon>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>

CompanyDialog::CompanyDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("Company Settings") + " [*]");
  setSizeGripEnabled(true);
  setMinimumSize(780, 540);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QScrollArea *m_centralWidget = new QScrollArea(this);
  m_centralWidget->setWidgetResizable(true);
  m_editWidget = new CompanyWidget(m_centralWidget);
  m_centralWidget->setWidget(m_editWidget);
  layout->addWidget(m_centralWidget);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  QPushButton *btn_save = m_btnBox->button(QDialogButtonBox::Ok);
  btn_save->setIcon(AntiquaCRM::antiquaIcon("action-save"));
  btn_save->setText(tr("Complete"));
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_editWidget, SIGNAL(sendSaved(bool)), SLOT(closeDialog(bool)));
  connect(btn_save, SIGNAL(clicked()), m_editWidget, SLOT(saveConfig()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
}

void CompanyDialog::closeDialog(bool b) {
  if (b) {
    m_statusBar->showMessage(tr("Company data saving success!"));
    QTimer::singleShot(1000, this, SLOT(accept()));
    return;
  }
  m_statusBar->showMessage(tr("An error has occurred!"));
}

int CompanyDialog::exec() {
  if (!m_editWidget->loadConfig())
    return QDialog::Rejected;

return QDialog::exec();
}
