// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentreminder.h"
#include "texteditor.h"

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinterInfo>

PaymentReminder::PaymentReminder(QWidget *parent) : Printing{parent} {
  setObjectName("printing_payment_reminder");
  setWindowTitle(tr("Payment Reminder"));

  pdfButton->setEnabled(true);
  connect(pdfButton, SIGNAL(clicked()), SLOT(generatePdf()));
  connect(printButton, SIGNAL(clicked()), SLOT(openPrintDialog()));
  readConfiguration();
}

void PaymentReminder::constructSubject() {
  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderBrush(borderBrush());
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setBottomMargin(20);
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
  QString addr(companyData.value("COMPANY_SHORTNAME"));
  addr.append(" - ");
  addr.append(companyData.value("COMPANY_STREET"));
  addr.append(" - ");
  addr.append(companyData.value("COMPANY_LOCATION"));
  cursor.insertText(addr);
  // Invoice
  QTextTableCell tc01 = table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(tr("Payment Reminder"));
  // Customer Address
  QTextTableCell tc10 = table->cellAt(1, 0);
  tc10.setFormat(addressFormat());
  cursor = tc10.firstCursorPosition();
  cursor.insertText(p_customerAddress);
  // Delivery Infos
  QTextTableCell tc11 = table->cellAt(1, 1);
  tc11.setFormat(normalFormat());
  cursor = tc11.firstCursorPosition();
  cursor.insertText(tr("Invoice-ID") + ": ");
  cursor.insertText(p_invoiceId);
  cursor.insertText("\n");
  cursor.insertText(tr("Order-ID") + ": ");
  cursor.insertText(p_orderId);
  cursor.insertText("\n");
  cursor.insertText(tr("Customer-ID") + ": ");
  cursor.insertText(p_customerId);
}

void PaymentReminder::constructBody() {
  constructSubject();

  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderBrush(borderBrush());
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(5);
  QTextTable *m_headerTable = cursor.insertTable(1, 2, format);
  m_headerTable->setObjectName("billings_table");

  QVector<QTextLength> constraints;
  QTextLength::Type type(QTextLength::PercentageLength);

  QTextCharFormat titleText = normalFormat();
  QFont titleFont = titleText.font();
  titleFont.setBold(true);
  titleText.setFont(titleFont);

  QTextTableCell ce00 = m_headerTable->cellAt(0, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(titleText);
  cursor.insertText(companyData.value("COMPANY_PAYMENT_REMINDER_TITLE"));
  constraints.append(QTextLength(type, 80)); // 80%

  QTextTableCell ce01 = m_headerTable->cellAt(0, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  QDate d = QDate::currentDate();
  cursor.insertText(d.toString("dd.MM.yyyy"));
  constraints.append(QTextLength(type, 20)); // 20%

  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

  QTextTable *m_noteTable = cursor.insertTable(1, 1, format);
  QTextTableCell noteCell = m_noteTable->cellAt(0, 0);
  cursor = noteCell.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  QRegExp pattern("\\b\\s{2,}\\b");
  cursor.insertText(p_mainText.replace(pattern, " "));
  constraints.append(QTextLength(type, 100));

  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

  m_billingTable = cursor.insertTable(1, 4, format);
  m_billingTable->setObjectName("billings_table");

  QTextTableCellFormat cellFormat;
  cellFormat.setBorderBrush(borderBrush());
  cellFormat.setTopBorder(0);
  cellFormat.setBottomBorder(1);
  cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QTextTableCell bce00 = m_billingTable->cellAt(0, 0);
  cursor = bce00.firstCursorPosition();
  bce00.setFormat(cellFormat);
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(tr("Article"));

  QTextTableCell bce01 = m_billingTable->cellAt(0, 1);
  bce01.setFormat(cellFormat);
  cursor = bce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Designation"));

  QTextTableCell bce02 = m_billingTable->cellAt(0, 2);
  bce02.setFormat(cellFormat);
  cursor = bce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(tr("Quantity"));

  QTextTableCell bce03 = m_billingTable->cellAt(0, 3);
  bce03.setFormat(cellFormat);
  cursor = bce03.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(tr("Price"));

  body->document()->setModified(true);
}

void PaymentReminder::insertBilling(BillingInfo billing) {
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);
  p_including_VAT = billing.includeVat;
  if (billing.packagePrice > 0)
    p_packagePrice = billing.packagePrice;

  if (billing.taxValue != p_tax_value)
    p_tax_value = billing.taxValue;

  p_disable_VAT = billing.disableVat;

  QTextCursor cursor = body->textCursor();
  QTextTableCell ce00 = m_billingTable->cellAt(row, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(billing.articleid);

  QTextTableCell ce01 = m_billingTable->cellAt(row, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  QString title = billing.designation.left(42);
  cursor.insertText(title + "...");

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

bool PaymentReminder::insertSummaryTable() {
  QString text;
  QTextCursor cursor = body->textCursor();
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 3);

  QTextTableCellFormat cellFormat;
  cellFormat.setBorderBrush(borderBrush());
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  // BEGIN Mehrwertsteuer
  m_billingTable->mergeCells(row, 0, 1, 3);
  QTextTableCell tc0 = m_billingTable->cellAt(row, 0);
  tc0.setFormat(cellFormat);
  cursor = tc0.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  text = QString();
  if (p_disable_VAT) {
    cursor.insertText(tr("VAT"));
  } else {
    if (p_including_VAT) {
      text.append(tr("incl.") + " ");
    }
    text.append(QString::number(p_tax_value));
    text.append("% " + tr("VAT"));
    cursor.insertText(text);
  }

  QTextTableCell tc1 = m_billingTable->cellAt(row, 3);
  tc1.setFormat(cellFormat);
  cursor = tc1.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  if (p_disable_VAT) {
    cursor.setBlockFormat(alignCenter());
    cursor.insertText("-");
  } else {
    cursor.setBlockFormat(alignRight());
    qreal tax;
    if (p_including_VAT) {
      tax = inclVat(p_fullPrice, p_tax_value);
    } else {
      tax = addVat(p_fullPrice, p_tax_value);
    }
    text = QString::number(tax, 'f', 2);
    cursor.insertText(text + " " + p_currency);
  }
  // END

  // BEGIN Zwischensumme
  row++;
  m_billingTable->mergeCells(row, 0, 1, 3);
  QTextTableCell zc0 = m_billingTable->cellAt(row, 0);
  cursor = zc0.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  text = tr("Subtotal");
  cursor.insertText(text);

  QTextTableCell zc1 = m_billingTable->cellAt(row, 3);
  zc1.setFormat(cellFormat);
  cursor = zc1.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  text = QString::number(p_fullPrice, 'f', 2);
  text.append(" " + p_currency);
  cursor.insertText(text);
  // END

  // BEGIN Versandkosten
  row++;
  m_billingTable->mergeCells(row, 0, 1, 3);
  QTextTableCell vc0 = m_billingTable->cellAt(row, 0);
  cursor = vc0.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  text = tr("delivery cost");
  cursor.insertText(text);

  QTextTableCell vc1 = m_billingTable->cellAt(row, 3);
  vc1.setFormat(cellFormat);
  cursor = vc1.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  text = QString::number(p_packagePrice, 'f', 2);
  text.append(" " + p_currency);
  cursor.insertText(text);
  if (p_packagePrice > 0.01) {
    p_fullPrice += p_packagePrice;
  }
  // END
  return true;
}

void PaymentReminder::finalizeBillings() {
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
    p_fullPrice += addVat(p_fullPrice, p_tax_value);
  }
  QString str = QString::number(p_fullPrice, 'f', 2);
  str.append(" " + p_currency);
  cursor.insertText(str);
}

void PaymentReminder::setAdditionalInfo() {
  if (p_subText.isEmpty())
    p_subText = companyData.value("COMPANY_PAYMENT_REMINDER_ADDITIONAL");

  QTextCursor cursor = body->textCursor();
  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

  QTextTableFormat format = tableFormat();
  format.setBorderBrush(borderBrush());
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(10);

  QTextTable *m_noteTable = cursor.insertTable(1, 1, format);
  QTextTableCell noteCell = m_noteTable->cellAt(0, 0);
  cursor = noteCell.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(p_subText);

  QVector<QTextLength> constraints;
  QTextLength::Type type(QTextLength::PercentageLength);
  constraints.append(QTextLength(type, 100));
}

void PaymentReminder::setRegards() {
  QTextCursor cursor = body->textCursor();
  QTextBlockFormat bf;
  bf.setLeftMargin(40);
  bf.setAlignment(Qt::AlignLeft);
  cursor.setCharFormat(footerFormat());
  cursor.setBlockFormat(bf);
  cursor.insertText("\n\n" + tr("Sincerely") + "\n ");
  cursor.insertText(companyData.value("COMPANY_SHORTNAME"));
}

bool PaymentReminder::generateDocument(QPrinter *printer) {
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

  painter.translate(0, 0);
  htmlHead->drawContents(&painter, headerRect);
  painter.translate(0, headerRect.height());
  htmlBody->drawContents(&painter, bodyRect);
  painter.translate(0, yPosFooter);
  htmlFooter->drawContents(&painter, footerRect);
  painter.end();
  return true;
}

void PaymentReminder::generatePdf() {
  if (createPDF("reminder")) {
    emit statusMessage(tr("PDF File written."));
  } else {
    emit statusMessage(tr("PDF not generated"));
  }
}

void PaymentReminder::openPrintDialog() {
  if (createPDF("reminder")) {
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
  printer->setDocName("PaymentReminder");
  printer->setPrinterName(p_printerName);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)),
          SLOT(generateDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

void PaymentReminder::setPaymentInfo(qint64 orderId,    /* Bestellnummer */
                                     qint64 customerId, /* Kundennummer */
                                     qint64 invoiceId,  /* Rechnungsnummer */
                                     const QString &deliverNoteId) {
  if (orderId < 1) {
    warningMessageBox(tr("There is no Order-Id to generate this invoice!"));
    return;
  }
  p_orderId = AntiquaCRM::AUtil::zerofill(orderId);

  if (customerId < 1) {
    warningMessageBox(tr("There is no Customer Id to generate this invoice!"));
    return;
  }
  p_customerId = AntiquaCRM::AUtil::zerofill(customerId);

  if (invoiceId < 1) {
    warningMessageBox(tr("There is no Invoice Id to generate this invoice!"));
    return;
  }
  p_invoiceId = AntiquaCRM::AUtil::zerofill(invoiceId);

  if (deliverNoteId.isEmpty()) {
    warningMessageBox(tr("delivery note number is empty!"));
    return;
  }
  p_deliveryId = deliverNoteId;
}

void PaymentReminder::setMainText(const QString &txt) { p_mainText = txt; }

void PaymentReminder::setFinalText(const QString &txt) { p_subText = txt; }

int PaymentReminder::exec() {
  qWarning("you must call exec with params!");
  return QDialog::Rejected;
}

int PaymentReminder::exec(const QList<BillingInfo> &billing) {
  if (p_orderId.isEmpty()) {
    qFatal("you must call setPaymentInfo() before exec!");
    return QDialog::Rejected;
  } else if (p_customerId.isEmpty()) {
    qFatal("you must call setPaymentInfo() before exec!");
    return QDialog::Rejected;
  } else if (p_invoiceId.isEmpty()) {
    qFatal("you must call setPaymentInfo() before exec!");
    return QDialog::Rejected;
  } else if (p_deliveryId.isEmpty()) {
    qFatal("you must call setPaymentInfo() before exec!");
    return QDialog::Rejected;
  }

  constructHeader();
  constructFooter();
  constructBody();

  QListIterator<BillingInfo> it(billing);
  while (it.hasNext()) {
    BillingInfo d = it.next();
    insertBilling(d);
  }
  if (insertSummaryTable())
    finalizeBillings();

  setAdditionalInfo();
  setRegards();

  addPrinters();

  return Printing::exec();
}
