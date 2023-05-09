// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintdialog.h"
#include "aprintingpage.h"

#include <QLayout>

namespace AntiquaCRM {

APrintDialog::APrintDialog(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(600, 400);

  config = new AntiquaCRM::ASettings(this);

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

  m_toolBar = new QToolBar(this);
  layout->addWidget(m_toolBar);

  viewPort = new QScrollArea(this);
  viewPort->setWidgetResizable(true);
  layout->addWidget(viewPort);

  m_buttonBox = new QDialogButtonBox(this);
  m_buttonBox->addButton(tr("Setup"), QDialogButtonBox::ActionRole);
  m_buttonBox->addButton(QDialogButtonBox::Apply);
  m_buttonBox->addButton(QDialogButtonBox::Close);
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

void APrintDialog::setPrintingPage(AntiquaCRM::APrintingPage *page) {
  Q_CHECK_PTR(page);
  viewPort->setWidget(page);
  viewPort->update();
}

int APrintDialog::exec() {
  qWarning("Do not use a Printer dialog without option set!");
  return QDialog::Rejected;
}

} // namespace AntiquaCRM
