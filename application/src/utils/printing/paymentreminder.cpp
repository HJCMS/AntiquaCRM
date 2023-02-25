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
}

void PaymentReminder::constructSubject() {
  QTextCursor cursor = body->textCursor();
  QString subject = tr("Payment Reminder");

  // Table
  QTextTable *table = constructInvoiceTable(subject);
  int row = (table->rows() - 1);

  // Anschrift
  QTextTableCell addrCell = table->cellAt(row, 0);
  addrCell.setFormat(addressFormat());
  cursor = addrCell.firstCursorPosition();
  cursor.insertText(p_customerAddress);

  // Betreff Informationen
  QMap<qint8, QString> title;
  title.insert(0, tr("Invoice No."));
  title.insert(1, tr("Order No."));
  title.insert(2, tr("Costumer No."));
  QMap<qint8, QString> data;
  data.insert(0, p_invoiceId);
  data.insert(1, p_orderId);
  data.insert(2, p_customerId);

  QTextTableCell infoCell = table->cellAt(row, 1);
  infoCell.setFormat(normalFormat());
  cursor = infoCell.firstCursorPosition();
  QTextTable *child_table = cursor.insertTable(data.size(), 3, tableFormat());

  QMapIterator<qint8, QString> it(data);
  while (it.hasNext()) {
    it.next();
    // left
    QTextTableCell cl = child_table->cellAt(it.key(), 0);
    cl.setFormat(normalFormat());
    cursor = cl.firstCursorPosition();
    cursor.setBlockFormat(alignRight());
    cursor.insertText(title[it.key()]);
    // middle
    QTextTableCell cm = child_table->cellAt(it.key(), 1);
    cm.setFormat(normalFormat());
    cursor = cm.firstCursorPosition();
    cursor.setBlockFormat(alignCenter());
    cursor.insertText(":");
    // right
    QTextTableCell cr = child_table->cellAt(it.key(), 2);
    cr.setFormat(normalFormat());
    cursor = cr.firstCursorPosition();
    cursor.setBlockFormat(alignRight());
    cursor.insertText(it.value());
  }

  // Begin:BodyHeaderSubject
  QTextTableFormat headerFormat = tableFormat();
  headerFormat.setTopMargin(15); // Abstand zum Adressenkopf

  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  QTextTable *m_headerTable = cursor.insertTable(1, 2, headerFormat);

  QVector<QTextLength> headerConstraint;
  QTextLength::Type type(QTextLength::PercentageLength);

  QTextTableCell cr00 = m_headerTable->cellAt(0, 0);
  cursor = cr00.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.insertText(subject);
  headerConstraint.append(QTextLength(type, 70));

  QTextTableCell cr01 = m_headerTable->cellAt(0, 1);
  cursor = cr01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  QString dueDate(companyData.value("COMPANY_LOCATION_NAME"));
  dueDate.append(" " + tr("on") + " ");
  dueDate.append(QDate::currentDate().toString("dd.MM.yyyy"));
  cursor.insertText(dueDate);
  headerConstraint.append(QTextLength(type, 30));
  // End:BodyHeaderSubject

  body->document()->setModified(true);
}

void PaymentReminder::constructBody() {
  constructSubject();

  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setTopMargin(5); // Abstand oben

  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  QTextTable *m_notify = cursor.insertTable(1, 1, format);
  cursor = m_notify->cellAt(0, 0).firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  QRegExp pattern("\\b\\s{2,}\\b");
  cursor.insertText(p_mainText.replace(pattern, " "));

  // Rechnungsaufstellung
  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  m_billingTable = cursor.insertTable(1, 4, format);

  QVector<QTextLength> billingConstraint;
  QTextLength::Type type(QTextLength::PercentageLength);

  QTextTableCell ce00 = m_billingTable->cellAt(0, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.insertText(tr("Article"));
  billingConstraint.append(QTextLength(type, 15)); // 10%

  QTextTableCell ce01 = m_billingTable->cellAt(0, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.insertText(tr("Designation"));
  billingConstraint.append(QTextLength(type, 60)); // 75%

  QTextTableCell ce02 = m_billingTable->cellAt(0, 2);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(tr("Quantity"));
  billingConstraint.append(QTextLength(type, 10)); // 85%

  QTextTableCell ce03 = m_billingTable->cellAt(0, 3);
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(tr("Price"));
  billingConstraint.append(QTextLength(type, 15)); // 100%

  format.clearColumnWidthConstraints();
  format.setColumnWidthConstraints(billingConstraint);
  m_billingTable->setFormat(format);
  body->document()->setModified(true);
}

bool PaymentReminder::insertDeliveryCost() {
  QTextCursor cursor = body->textCursor();
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);

  // BEGIN Versandkosten
  m_billingTable->mergeCells(row, 0, 1, 3);
  QTextTableCell vc0 = m_billingTable->cellAt(row, 0);
  vc0.setFormat(cellFormat(Printing::Border::Top));
  cursor = vc0.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(tr("delivery cost"));

  QTextTableCell vc1 = m_billingTable->cellAt(row, 3);
  vc1.setFormat(cellFormat(Printing::Border::Top));
  cursor = vc1.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(money(p_deliveryCost));
  if (p_deliveryCost > 0.1) {
    p_totalPrice += p_deliveryCost;
  }
  // END
  return true;
}

void PaymentReminder::finalizeBillings() {
  int row = m_billingTable->rows();
  m_billingTable->insertRows(row, 1);
  m_billingTable->mergeCells(row, 0, 1, 3);

  QTextCursor cursor = body->textCursor();
  QTextTableCell infoCell = m_billingTable->cellAt(row, 0);
  infoCell.setFormat(cellFormat(Printing::Border::NoBorder));
  cursor = infoCell.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(tr("invoice amount"));

  QTextTableCell costCell = m_billingTable->cellAt(row, 3);
  costCell.setFormat(cellFormat(Printing::Border::Top));
  cursor = costCell.firstCursorPosition();
  cursor.setCharFormat(boldFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(money(p_totalPrice));
}

void PaymentReminder::setAdditionalInfo() {
  if (p_subText.isEmpty())
    p_subText = companyData.value("COMPANY_PAYMENT_REMINDER_ADDITIONAL");

  QTextCursor cursor = body->textCursor();
  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

  QTextTableFormat format = tableFormat();
  format.setTopMargin(10);
  format.setLeftMargin(p_margins.left());

  QTextTable *m_noteTable = cursor.insertTable(1, 1, format);
  QTextTableCell noteCell = m_noteTable->cellAt(0, 0);
  cursor = noteCell.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(p_subText);

  QVector<QTextLength> constraints;
  QTextLength::Type type(QTextLength::PercentageLength);
  constraints.append(QTextLength(type, 100));
}

bool PaymentReminder::generateDocument(QPrinter *printer) {
  QRectF pageRect(printArea->geometry());
  int documentWidth = pageRect.width();

  QImage image = getWatermark();
  QPainter painter;
  painter.begin(printer);
  painter.setWindow(printArea->rect());
  painter.setPen(QPen(Qt::darkGray, Qt::SolidLine));

  if (!image.isNull()) {
    painter.translate(0, 0);
    painter.setOpacity(0.5);
    painter.drawImage(QPoint((p_margins.left() / 2), 0), image);
    painter.setOpacity(1.0);
  }

  // helper lines
  int _yh = (pageRect.height() / 3);
  int _ym = (pageRect.height() / 2);
  int _len = (p_margins.left() - 5);
  painter.translate(0, 0);
  painter.drawLine(QPoint(5, _yh), QPoint(_len, _yh));
  painter.drawLine(QPoint(5, _ym), QPoint(_len, _ym));

  QTextDocument *htmlHead = header->document();
  htmlHead->setHtml(getHeaderHTML());
  htmlHead->setPageSize(QSizeF(documentWidth, header->size().height()));
  htmlHead->setModified(true);
  QRectF headerRect = QRectF(QPointF(0, 0), htmlHead->pageSize());

  painter.translate(0, 0);
  htmlHead->drawContents(&painter, headerRect);

  QTextDocument *htmlBody = body->document();
  htmlBody->setHtml(getBodyHTML());
  htmlBody->setPageSize(QSizeF(documentWidth, body->size().height()));
  htmlBody->setModified(true);
  QRectF bodyRect = QRectF(QPointF(0, 0), htmlBody->pageSize());

  painter.translate(0, headerRect.height());
  htmlBody->drawContents(&painter, bodyRect);

  QTextDocument *htmlFooter = footer->document();
  htmlFooter->setHtml(getFooterHTML());
  htmlFooter->setPageSize(QSizeF(documentWidth, footer->size().height()));
  htmlFooter->setModified(true);
  QRectF footerRect = QRectF(QPointF(0, 0), htmlFooter->pageSize());
  int yPosFooter = (pageRect.height() - (footerRect.height() * 2));

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

void PaymentReminder::setPaymentInfo(qint64 orderId,     /* Bestellnummer */
                                     qint64 customerId,  /* Kundennummer */
                                     qint64 invoiceId,   /* Rechnungsnummer */
                                     qreal deliverprice, /* Paketsender Preis */
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
  p_deliveryCost = deliverprice;
}

void PaymentReminder::setMainText(const QString &txt) { p_mainText = txt; }

void PaymentReminder::setFinalText(const QString &txt) { p_subText = txt; }

int PaymentReminder::exec() {
  qWarning("you must call exec with params!");
  return QDialog::Rejected;
}

int PaymentReminder::exec(const QList<BillingInfo> &list) {
  readConfiguration();

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

  if (list.size() < 1) {
    qWarning("Missing billing infos!");
    return QDialog::Rejected;
  }

  constructHeader();
  constructFooter();
  constructBody();

  QListIterator<BillingInfo> it(list);
  while (it.hasNext()) {
    addArticleRow(m_billingTable, it.next());
  }
  body->document()->setModified(true);

  if (insertDeliveryCost())
    finalizeBillings();

  setAdditionalInfo();

  QStringList regards(tr("Sincerely"));
  regards.append(companyData.value("COMPANY_EMPLOYER"));
  setRegards(regards);
  body->document()->setModified(true);

  addPrinters();

  return Printing::exec();
}
