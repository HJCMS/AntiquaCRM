// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printreport.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QPrintDialog>

namespace AntiquaCRM {

PrintReport::PrintReport(QWidget *parent) : AntiquaCRM::APrintDialog{parent} {
  setObjectName("print_report_dialog");
  page = new APrintingBody(this);
  viewPort->setWidget(page);
}

void PrintReport::renderPage(QPrinter *printer) {
  Q_CHECK_PTR(page);
  QPainter _painter(printer);
  _painter.setWindow(page->rect());
  _painter.translate(0, 0);
  page->render(&_painter);
  _painter.end();
}

void PrintReport::createPDF() {
  const QDir _dir = config->getArchivPath(ANTIQUACRM_ARCHIVE_REPORTS);
  if (!_dir.exists()) {
    qWarning("Bookcard destination not exists or writeable!");
    return;
  }

  QFileInfo _file(_dir, pdfFileName);
  QPrinter *printer = new QPrinter(QPrinter::HighResolution);
  printer->setPageLayout(pageLayout);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setCreator("AntiquaCRM");
  printer->setOutputFileName(_file.filePath());
  renderPage(printer);
  if (_file.isReadable())
    sendStatusMessage(tr("PDF Document created!"));
}

void PrintReport::openPrintDialog() {
  QPrinterInfo p_info;
  QString _device = config->value("printer/device_secondary").toString();
  if (_device.isEmpty()) {
    p_info = QPrinterInfo::defaultPrinter();
  } else {
    p_info = QPrinterInfo::printerInfo(_device);
  }

  QPrinter *printer = new QPrinter(p_info, QPrinter::ScreenResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setDocName(pdfFileName);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  dialog->setOption(QAbstractPrintDialog::PrintShowPageSize, true);

  connect(dialog, SIGNAL(accepted(QPrinter *)), SLOT(renderPage(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

int PrintReport::exec(const QJsonObject &opts, bool pdfbtn) {
  btn_pdf->setEnabled(pdfbtn);
  pdfFileName = opts.value("basename").toString();
  pdfFileName.append(".pdf");

  QJsonDocument _doc(opts);
  page->setText(_doc.toJson());

  update();

  return QDialog::exec();
}

} // namespace AntiquaCRM
