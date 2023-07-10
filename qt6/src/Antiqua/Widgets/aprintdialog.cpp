// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintdialog.h"
#include "ainputwidget.h"
#include "aprintingpage.h"

#include <QLayout>

namespace AntiquaCRM {

APrintDialog::APrintDialog(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(330, 480);
  setContentsMargins(5, 5, 5, 5);

  config = new AntiquaCRM::ASettings(this);
  pdfFileName = "unknown.pdf";

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->setContentsMargins(2, 2, 2, 2);

  viewPort = new QScrollArea(this);
  viewPort->setWidgetResizable(true);
  layout->addWidget(viewPort);

  m_buttonBox = new QDialogButtonBox(this);

  QPushButton *btn_pdf = new QPushButton(this);
  btn_pdf->setText(tr("PDF"));
  btn_pdf->setIcon(AntiquaCRM::antiquaIcon("application-pdf"));
  btn_pdf->setToolTip(tr("Generate PDF."));
  m_buttonBox->addButton(btn_pdf, QDialogButtonBox::ActionRole);

  QPushButton *btn_print = new QPushButton(this);
  btn_print->setText(tr("Print"));
  btn_print->setIcon(AntiquaCRM::antiquaIcon("printer"));
  btn_print->setToolTip(tr("Print this view."));
  m_buttonBox->addButton(btn_print, QDialogButtonBox::ApplyRole);

  m_buttonBox->addButton(QDialogButtonBox::Close);
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(btn_print, SIGNAL(clicked()), SLOT(openPrintDialog()));
  connect(btn_pdf, SIGNAL(clicked()), SLOT(createPDF()));
  connect(m_buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

void APrintDialog::setPrintingPage(AntiquaCRM::APrintingPage *page) {
  Q_CHECK_PTR(page);
  viewPort->setWidget(page);
  viewPort->update();
}

void APrintDialog::sendStatusMessage(const QString &message) {
  m_statusBar->showMessage(message, 5000);
}

int APrintDialog::exec() {
  qWarning("Do not use a Printer dialog without options set!");
  return QDialog::Rejected;
}

} // namespace AntiquaCRM
