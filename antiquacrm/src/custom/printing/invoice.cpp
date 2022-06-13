// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "invoice.h"
#include "applsettings.h"
#include "texteditor.h"

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>

Invoice::Invoice(QWidget *parent) : Printing{parent} {
  setObjectName("printing_invoice");

  connect(printButton, SIGNAL(clicked()), this, SLOT(openPrintDialog()));
  readConfiguration();
}

void Invoice::constructSubject() {
  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(30);
  QTextTable *table = cursor.insertTable(2, 2, format);
  table->setObjectName("address_table");
  // Company Address
  QTextCharFormat cellFormat;
  QFont font(getSmallFont());
  font.setUnderline(true);
  font.setPointSize(8);
  cellFormat.setFont(font);
  cellFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);
  QTextTableCell tc00 = table->cellAt(0, 0);
  tc00.setFormat(cellFormat);
  cursor = tc00.firstCursorPosition();
  QString addr(companyData.value("shortname"));
  addr.append(" - ");
  addr.append(companyData.value("street"));
  addr.append(" - ");
  addr.append(companyData.value("location"));
  cursor.insertText(addr);
  // Invoice
  QTextTableCell tc01 = table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(tr("Invoice"));
  // Customer Address
  QTextTableCell tc10 = table->cellAt(1, 0);
  tc10.setFormat(normalFormat());
  cursor = tc10.firstCursorPosition();
  cursor.insertText(p_customerAddress);
  // Delivery Infos
  QTextTableCell tc11 = table->cellAt(1, 1);
  tc11.setFormat(normalFormat());
  cursor = tc11.firstCursorPosition();
  cursor.insertText(tr("Invoice-ID:") + " ");
  cursor.insertText(p_invoiceId);
  cursor.insertText("\n");
  cursor.insertText(tr("Order-ID:") + " ");
  cursor.insertText(p_orderId);
  cursor.insertText("\n");
  cursor.insertText(tr("Customer-ID:") + " ");
  cursor.insertText(p_customerId);
  cursor.insertText("\n");
  cursor.insertText(tr("Delivery-ID:") + " ");
  cursor.insertText(p_deliveryId);
  cursor.insertText("\n");
  body->document()->setModified(true);
}

void Invoice::constructBody() {
  constructSubject();

  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(30);
  m_billingTable = cursor.insertTable(1, 4, format);
  m_billingTable->setObjectName("billings_table");

  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderBrush(Qt::SolidPattern);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  cellFormat.setBottomBorder(1);
  cellFormat.setBottomBorderBrush(Qt::SolidPattern);
  cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QTextTableCell ce00 = m_billingTable->cellAt(0, 0);
  cursor = ce00.firstCursorPosition();
  ce00.setFormat(cellFormat);
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Article-ID"));

  QTextTableCell ce01 = m_billingTable->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Designation"));

  QTextTableCell ce02 = m_billingTable->cellAt(0, 2);
  ce02.setFormat(cellFormat);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Quantity"));

  QTextTableCell ce03 = m_billingTable->cellAt(0, 3);
  ce03.setFormat(cellFormat);
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Price"));

  body->document()->setModified(true);
}

void Invoice::insertBilling(const QString &articleid,
                            const QString &designation, int quantity,
                            double sellPrice) {
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);

  QTextCursor cursor = body->textCursor();
  QTextTableCell ce00 = m_billingTable->cellAt(row, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(articleid);

  QTextTableCell ce01 = m_billingTable->cellAt(row, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(designation);

  QTextTableCell ce02 = m_billingTable->cellAt(row, 2);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(QString::number(quantity));

  QTextTableCell ce03 = m_billingTable->cellAt(row, 3);
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  double price = sellPrice;
  if (quantity > 1) {
    price = (quantity * sellPrice);
  }
  p_fullPrice += price;
  QString str = QString::number(price, 'f', 2);
  cursor.insertText(str + " " + p_currency);

  body->document()->setModified(true);
}

void Invoice::finalizeBillings() {
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);

  QTextCursor cursor = body->textCursor();
  QTextTableCell ce02 = m_billingTable->cellAt(row, 2);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Summary:"));

  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderBrush(Qt::SolidPattern);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QTextTableCell ce03 = m_billingTable->cellAt(row, 3);
  cursor = ce03.firstCursorPosition();
  ce03.setFormat(cellFormat);
  cursor.setCharFormat(normalFormat());
  QString str = QString::number(p_fullPrice, 'f', 2);
  cursor.insertText(str + " " + p_currency);

  body->document()->setModified(true);
}

bool Invoice::createPDF() {
  QPrinter *printer = new QPrinter(QPrinter::PrinterResolution);
  QString dest = outputDirectory("invoices");
  dest.append(QDir::separator());
  dest.append(p_invoiceId);
  dest.append(".pdf");
  printer->setOutputFileName(dest);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setPageLayout(pdfLayout());
  return generateDocument(printer);
}

bool Invoice::generateDocument(QPrinter *printer) {
  QRectF pageRect = printer->pageRect(QPrinter::Point);
  int border = printer->pageLayout().margins().left();
  int documentWidth = qRound(pageRect.size().width() - (border * 2));

  QTextDocument *htmlHead = header->document();
  htmlHead->setHtml(getHeaderHTML());
  htmlHead->setPageSize(QSizeF(documentWidth, header->size().height()));
  htmlHead->setModified(true);
  QRectF headerRect = QRectF(QPoint(0, 0), htmlHead->pageSize());

  QTextDocument *htmlBody = body->document();
  htmlBody->setHtml(getBodyHTML());
  htmlBody->setPageSize(QSizeF(documentWidth, body->size().height()));
  htmlBody->setModified(true);
  QRectF bodyRect = QRectF(QPoint(0, 0), htmlBody->pageSize());

  QTextDocument *htmlFooter = footer->document();
  htmlFooter->setHtml(getFooterHTML());
  htmlFooter->setPageSize(QSizeF(documentWidth, footer->size().height()));
  htmlFooter->setModified(true);
  QRectF footerRect = QRectF(QPoint(0, 0), htmlFooter->pageSize());
  int yPosFooter = (pageRect.height() - (footerRect.height() * 2));

  /*
  qDebug() << Q_FUNC_INFO  << printer->printerName() << Qt::endl
           << "Head:" << headerRect << Qt::endl
           << "Body:" << bodyRect << Qt::endl
           << "Footer." << footerRect;
  */

  QImage image = getWatermark();
  QPainter painter;
  painter.begin(printer);
  if (!image.isNull()) {
    painter.translate(0, 0);
    painter.setOpacity(0.3);
    painter.drawImage(QPoint(0, 0), image);
    painter.setOpacity(1.0);
  }

  int pY = 0;
  painter.translate(border, 0);
  htmlHead->drawContents(&painter, headerRect);
  painter.translate(border, headerRect.height());
  htmlBody->drawContents(&painter, bodyRect);
  painter.translate(border, yPosFooter);
  htmlFooter->drawContents(&painter, footerRect);
  painter.end();
  return true;
}

void Invoice::openPrintDialog() {
  if (createPDF()) {
    emit statusMessage(tr("PDF File written."));
  } else {
    emit statusMessage(tr("PDF not generated"));
  }

  if (p_printerName.isEmpty()) {
    emit statusMessage(tr("No Printer found"));
    return;
  }

  QPrinter *printer = new QPrinter(QPrinter::PrinterResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout());
  printer->setPrinterName(p_printerName);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)), this,
          SLOT(generateDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

void Invoice::setInvoice(int orderId,    /* Bestellnummer */
                         int customerId, /* Kundennummer */
                         int invoiceId,  /* Rechnungsnummer */
                         const QString &deliverNoteId) {
  if (orderId < 1) {
    warningMessageBox(tr("There is no Order-Id to generate this invoice!"));
    return;
  }
  p_orderId = QString::number(orderId).rightJustified(7, '0');

  if (customerId < 1) {
    warningMessageBox(tr("There is no Customer Id to generate this invoice!"));
    return;
  }
  p_customerId = QString::number(customerId).rightJustified(7, '0');

  if (invoiceId < 1) {
    warningMessageBox(tr("There is no Invoice Id to generate this invoice!"));
    return;
  }
  p_invoiceId = QString::number(invoiceId).rightJustified(7, '0');

  if (deliverNoteId.isEmpty()) {
    warningMessageBox(tr("delivery note number is empty!"));
    return;
  }
  p_deliveryId = deliverNoteId;
}

int Invoice::exec(const QList<BillingInfo> &list) {
  if (p_orderId.isEmpty()) {
    qFatal("you must call setInvoice() before exec!");
    return QDialog::Rejected;
  } else if (p_customerId.isEmpty()) {
    qFatal("you must call setInvoice() before exec!");
    return QDialog::Rejected;
  } else if (p_invoiceId.isEmpty()) {
    qFatal("you must call setInvoice() before exec!");
    return QDialog::Rejected;
  } else if (p_deliveryId.isEmpty()) {
    qFatal("you must call setInvoice() before exec!");
    return QDialog::Rejected;
  }

  p_currency = config->value("payment/currency").toString();

  constructHeader();
  constructFooter();
  constructBody();

  QListIterator<BillingInfo> it(list);
  while (it.hasNext()) {
    BillingInfo d = it.next();
    insertBilling(d.articleid, d.designation, d.quantity, d.sellPrice);
  }

  finalizeBillings();
  body->setReadOnly(true);

  addPrinters();

  return Printing::exec();
}
