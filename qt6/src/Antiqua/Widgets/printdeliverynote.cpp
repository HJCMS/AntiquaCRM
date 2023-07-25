// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

#include <QPrintDialog>

namespace AntiquaCRM {

DeliveryNote::DeliveryNote(QWidget *parent)
    : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_deliverynote_page");
}

PrintDeliveryNote::PrintDeliveryNote(QWidget *parent) : APrintDialog{parent} {
  setObjectName("print_deliverynote_dialog");
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(QMargins(0, 0, 0, 0));
  pageLayout.setUnits(QPageLayout::Millimeter);
  pageLayout.setMode(QPageLayout::FullPageMode);
}

void PrintDeliveryNote::renderPage(QPrinter *printer) {
  Q_CHECK_PTR(page);
  QPainter painter(printer);
  painter.setWindow(page->rect());
  painter.translate(0, 0);
  page->render(&painter);
  painter.end();
}

void PrintDeliveryNote::createPDF() {
  QDir _dir(config->value("dirs/archive_delivery").toString());
  if (_dir.exists()) {
    QFileInfo _file(_dir, pdfFileName);
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageLayout(pageLayout);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setCreator("AntiquaCRM");
    printer->setOutputFileName(_file.filePath());
    renderPage(printer);
    if (_file.isReadable())
      sendStatusMessage(tr("Delivery note - PDF created!"));
  }
}

void PrintDeliveryNote::openPrintDialog() {
  const QPrinterInfo printerInfo = page->getPrinterInfo();
  QPageLayout pageLayout = page->pageLayout();
  pageLayout.setMode(QPageLayout::FullPageMode);

  QPrinter *printer = new QPrinter(printerInfo, QPrinter::PrinterResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout);
  printer->setDocName("Invoice");
  printer->setPrinterName(printerInfo.printerName());
  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)), SLOT(renderPage(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
    sendStatusMessage(tr("Delivery note printed!"));
  }
}

int PrintDeliveryNote::exec(const QJsonObject &options) {
  qint64 o_id = options.value("order_id").toInt();
  if (o_id < 1) {
    qWarning("Missing Order ID.");
    return QDialog::Rejected;
  }

  qint64 c_id = options.value("customer_id").toInt();
  if (c_id < 1) {
    qWarning("Missing Customer ID.");
    return QDialog::Rejected;
  }

  const QString de_id = options.value("delivery_id").toString();
  if (de_id.length() < 3) {
    qWarning("Missing Delivery ID.");
    return QDialog::Rejected;
  }

  qDebug() << Q_FUNC_INFO << options;

  pdfFileName = AntiquaCRM::AUtil::zerofill(o_id);
  pdfFileName.append(".pdf");

  page = new DeliveryNote(this);
  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);
  page->setLetterHeading(tr("Delivery note"));
  page->setRecipientAddress(_person.value("address").toString(), options);
  page->setLetterSubject(tr("Delivery note"));
  setPrintingPage(page);

  return QDialog::exec();
}

} // namespace AntiquaCRM
