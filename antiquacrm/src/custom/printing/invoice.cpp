// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "invoice.h"
#include "applsettings.h"
#include "texteditor.h"

#include <QBrush>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinterInfo>

Invoice::Invoice(QWidget *parent) : Printing{parent} {
  setObjectName("printing_invoice");
  connect(printButton, SIGNAL(clicked()), this, SLOT(openPrintDialog()));
  readConfiguration();
}

void Invoice::constructSubject() {
  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderBrush(borderBrush());
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
  format.setBorderBrush(borderBrush());
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  // format.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  format.setTopMargin(20);
  m_billingTable = cursor.insertTable(1, 4, format);
  m_billingTable->setObjectName("billings_table");

  QFontMetricsF fm(normalFormat().font());

  QTextTableCellFormat cellFormat;
  cellFormat.setBorderBrush(borderBrush());
  cellFormat.setTopBorder(0);
  cellFormat.setBottomBorder(1);
  cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QVector<QTextLength> constraints;
  QTextLength::Type type(QTextLength::PercentageLength);

  QString txt;
  QTextTableCell ce00 = m_billingTable->cellAt(0, 0);
  cursor = ce00.firstCursorPosition();
  ce00.setFormat(cellFormat);
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  txt = tr("Article");
  cursor.insertText(txt);
  constraints.append(QTextLength(type, 15)); // 15%

  QTextTableCell ce01 = m_billingTable->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  txt = tr("Designation");
  cursor.insertText(txt);
  constraints.append(QTextLength(type, 60)); // 60%

  QTextTableCell ce02 = m_billingTable->cellAt(0, 2);
  ce02.setFormat(cellFormat);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  txt = tr("Quantity");
  cursor.insertText(txt);
  constraints.append(QTextLength(type, 10)); // 10%

  QTextTableCell ce03 = m_billingTable->cellAt(0, 3);
  ce03.setFormat(cellFormat);
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  txt = tr("Price");
  cursor.insertText(txt);
  constraints.append(QTextLength(type, 15)); // 15%

  format.clearColumnWidthConstraints();
  format.setColumnWidthConstraints(constraints);
  m_billingTable->setFormat(format);

  body->document()->setModified(true);
}

void Invoice::insertBilling(BillingInfo billing) {
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);
  p_including_VAT = billing.includeVat;
  if (billing.packagePrice > 0)
    p_packagePrice = billing.packagePrice;

  if (billing.taxValue != p_tax_value)
    p_tax_value = billing.taxValue;

  QTextCursor cursor = body->textCursor();
  QTextTableCell ce00 = m_billingTable->cellAt(row, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(billing.articleid);

  QTextTableCell ce01 = m_billingTable->cellAt(row, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(billing.designation);

  QTextTableCell ce02 = m_billingTable->cellAt(row, 2);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  p_quantity_sum += billing.quantity;
  cursor.insertText(QString::number(billing.quantity));

  QTextTableCell ce03 = m_billingTable->cellAt(row, 3);
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  double price = billing.sellPrice;
  if (billing.quantity > 1) {
    price = (billing.quantity * billing.sellPrice);
  }
  p_fullPrice += price;
  QString str = QString::number(price, 'f', 2);
  cursor.insertText(str + " " + p_currency);

  body->document()->setModified(true);
}

bool Invoice::insertSummaryTable() {
  QTextCursor cursor = body->textCursor();
  int row = m_billingTable->rows();
  int addRows = (p_packagePrice > 0) ? 2 : 1;
  m_billingTable->insertRows(row, addRows);

  QTextTableCellFormat cellFormat;
  cellFormat.setBorderBrush(borderBrush());
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  m_billingTable->mergeCells(row, 0, 1, 3);
  if (addRows > 1) {
    QTextTableCell left = m_billingTable->cellAt(row, 0);
    cursor = left.firstCursorPosition();
    cursor.setCharFormat(normalFormat());
    cursor.setBlockFormat(alignRight());
    QString str(tr("Package delivery cost"));
    cursor.insertText(str);

    QTextTableCell right = m_billingTable->cellAt(row, 3);
    right.setFormat(cellFormat);
    cursor = right.firstCursorPosition();
    cursor.setCharFormat(normalFormat());
    cursor.setBlockFormat(alignRight());
    QString cost(QString::number(p_packagePrice, 'f', 2));
    cost.append(" " + p_currency);
    cursor.insertText(cost);

    p_fullPrice += p_packagePrice;
    row++;
    m_billingTable->mergeCells(row, 0, 1, 3);
  }

  QTextTableCell left = m_billingTable->cellAt(row, 0);
  cursor = left.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  QString tax = QString::number(p_tax_value);
  QString strTax;
  if (p_including_VAT) {
    strTax.append(tr("incl.") + " ");
  }
  strTax.append(tax + "% " + tr("VAT"));
  cursor.insertText(strTax);

  QTextTableCell right = m_billingTable->cellAt(row, 3);
  right.setFormat(cellFormat);
  cursor = right.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  if (!p_including_VAT) {
    qreal vat = ((p_fullPrice / (100 + p_tax_value)) * p_tax_value);
    QString str = QString::number(vat, 'f', 2);
    cursor.insertText(str + " " + p_currency);
  }
  body->document()->setModified(true);
  return true;
}

void Invoice::finalizeBillings() {
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);
  m_billingTable->mergeCells(row, 0, 1, 3);

  QTextTableCellFormat cellFormat;
  cellFormat.setBorderBrush(borderBrush());
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QTextCursor cursor = body->textCursor();
  QTextTableCell infoCell = m_billingTable->cellAt(row, 0);
  cursor = infoCell.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  QString summary(tr("invoice amount"));
  cursor.insertText(summary);

  QTextTableCell costCell = m_billingTable->cellAt(row, 3);
  costCell.setFormat(cellFormat);
  cursor = costCell.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  if (!p_including_VAT) {
    p_fullPrice += ((p_fullPrice / (100 + p_tax_value)) * p_tax_value);
  }
  QString str = QString::number(p_fullPrice, 'f', 2);
  str.append(" " + p_currency);
  cursor.insertText(str);

  body->document()->setModified(true);
}

void Invoice::setComment(const QString &msg) {
  QTextCursor cursor = body->textCursor();
  qreal blockMargin = 30.0;
  QTextBlockFormat bf;
  bf.setTopMargin(blockMargin);
  bf.setLeftMargin(blockMargin);
  bf.setRightMargin(blockMargin);
  bf.setAlignment(Qt::AlignLeft);
  cursor.setCharFormat(smallFormat());
  cursor.setBlockFormat(bf);
  cursor.insertText("\n");
  cursor.insertText(msg);
  body->document()->setModified(true);
}

bool Invoice::generateDocument(QPrinter *printer) {
  QRectF pageRect(printArea->geometry());
  int documentWidth = pageRect.width();

  QTextDocument *htmlHead = header->document();
  htmlHead->setHtml(getHeaderHTML());
  htmlHead->setPageSize(QSizeF(documentWidth, header->size().height()));
  htmlHead->setModified(true);
  QRectF headerRect = QRectF(QPointF(0, 0), htmlHead->pageSize());

  QTextDocument *htmlBody = body->document();
  htmlBody->setHtml(getBodyHTML());
  htmlBody->setPageSize(QSizeF(documentWidth, body->size().height()));
  htmlBody->setModified(true);
  QRectF bodyRect = QRectF(QPointF(0, 0), htmlBody->pageSize());

  QTextDocument *htmlFooter = footer->document();
  htmlFooter->setHtml(getFooterHTML());
  htmlFooter->setPageSize(QSizeF(documentWidth, footer->size().height()));
  htmlFooter->setModified(true);
  QRectF footerRect = QRectF(QPointF(0, 0), htmlFooter->pageSize());
  int yPosFooter = (pageRect.height() - (footerRect.height() * 2));

  QImage image = getWatermark();
  QPainter painter;
  painter.begin(printer);
  painter.setWindow(printArea->rect());

  if (!image.isNull()) {
    painter.translate(0, 0);
    painter.setOpacity(0.5);
    painter.drawImage(QPoint(0, 0), image);
    painter.setOpacity(1.0);
  }

  int pY = 0;
  painter.translate(0, 0);
  htmlHead->drawContents(&painter, headerRect);
  painter.translate(0, headerRect.height());
  htmlBody->drawContents(&painter, bodyRect);
  painter.translate(0, yPosFooter);
  htmlFooter->drawContents(&painter, footerRect);
  painter.end();
  return true;
}

void Invoice::openPrintDialog() {
  if (createPDF("invoices")) {
    emit statusMessage(tr("PDF File written."));
  } else {
    emit statusMessage(tr("PDF not generated"));
  }

  if (p_printerName.isEmpty()) {
    emit statusMessage(tr("No Printer found"));
    return;
  }

  QPrinterInfo p_info = QPrinterInfo::printerInfo(p_printerName);
  QPrinter *printer = new QPrinter(p_info, QPrinter::PrinterResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout());
  printer->setFullPage(true);
  printer->setDocName("Invoice");
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

int Invoice::exec(const QList<BillingInfo> &list, const QString &comment) {
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

  constructHeader();
  constructFooter();
  constructBody();

  QListIterator<BillingInfo> it(list);
  while (it.hasNext()) {
    BillingInfo d = it.next();
    insertBilling(d);
  }
  if (insertSummaryTable())
    finalizeBillings();

  if (!comment.isEmpty())
    setComment(comment);

  addPrinters();

  return Printing::exec();
}
