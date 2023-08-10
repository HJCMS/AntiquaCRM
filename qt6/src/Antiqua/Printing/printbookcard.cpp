// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printbookcard.h"
#include "bookcardpage.h"

#include <QPrintDialog>

namespace AntiquaCRM {

PrintBookCard::PrintBookCard(QWidget *parent)
    : AntiquaCRM::APrintDialog{parent} {
  setObjectName("print_bookcard_dialog");
  setMinimumSize(300, 500);
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A6));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(QMargins(0, 0, 0, 0));
  pageLayout.setUnits(QPageLayout::Millimeter);
  pageLayout.setMode(QPageLayout::FullPageMode);
}

bool PrintBookCard::notValid(const QJsonValue &value) const {
  switch (value.type()) {
  case (QJsonValue::Double):
    return (value.toInteger(0) == 0);

  case (QJsonValue::String):
    return (value.toString().length() < 1);

  default: // null or unknown = not valid
    return true;
  }
  return true;
}

void PrintBookCard::renderPage(QPrinter *printer) {
  Q_CHECK_PTR(page);
  QPainter _painter(printer);
  _painter.setWindow(page->rect());
  _painter.translate(0, 0);
  page->render(&_painter);
  _painter.end();
}

void PrintBookCard::createPDF() {
  QDir _dir = config->getArchivPath(ANTIQUACRM_ARCHIVE_CARDS);
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

void PrintBookCard::openPrintDialog() {
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

int PrintBookCard::exec(const QJsonObject &opts) {
  if (!opts.contains("aid") || !opts.contains("basename")) {
    qWarning("Missing Article Id!");
    return QDialog::Rejected;
  }

  if (notValid(opts.value("aid")) || notValid(opts.value("basename"))) {
    qWarning("Missing Article Id!");
    return QDialog::Rejected;
  }

  pdfFileName = opts.value("basename").toString();
  pdfFileName.append(".pdf");

  // #ifdef ANTIQUA_DEVELOPEMENT
  //   qDebug() << Q_FUNC_INFO << opts;
  // #endif

  page = new BookCardPage(opts, this);
  viewPort->setWidget(page);
  update();

  return QDialog::exec();
}

} // namespace AntiquaCRM
