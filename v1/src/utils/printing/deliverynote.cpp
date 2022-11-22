// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverynote.h"
#include "texteditor.h"

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinterInfo>

DeliveryNote::DeliveryNote(QWidget *parent) : Printing{parent} {
  setObjectName("printing_delivery_note");
  setWindowTitle(tr("Delivery note"));

  pdfButton->setEnabled(true);
  connect(pdfButton, SIGNAL(clicked()), SLOT(generatePdf()));
  connect(printButton, SIGNAL(clicked()), SLOT(openPrintDialog()));
  readConfiguration();
}

void DeliveryNote::constructSubject() {
  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
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
  // Delivey note
  QTextTableCell tc01 = table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(tr("Delivey note"));
  // Customer Address
  QTextTableCell tc10 = table->cellAt(1, 0);
  tc10.setFormat(addressFormat());
  cursor = tc10.firstCursorPosition();
  cursor.insertText(p_customerAddress);
  // Delivery Infos
  QTextTableCell tc11 = table->cellAt(1, 1);
  tc11.setFormat(normalFormat());
  cursor = tc11.firstCursorPosition();
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

void DeliveryNote::constructBody() {
  constructSubject();

  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(30);
  m_articleTable = cursor.insertTable(1, 3, format);
  m_articleTable->setObjectName("article_table");

  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderBrush(Qt::SolidPattern);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  cellFormat.setBottomBorder(1);
  cellFormat.setBottomBorderBrush(Qt::SolidPattern);
  cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QTextTableCell ce00 = m_articleTable->cellAt(0, 0);
  cursor = ce00.firstCursorPosition();
  ce00.setFormat(cellFormat);
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Article-ID"));

  QTextTableCell ce01 = m_articleTable->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Designation"));

  QTextTableCell ce02 = m_articleTable->cellAt(0, 2);
  ce02.setFormat(cellFormat);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Quantity"));
  body->document()->setModified(true);
}

void DeliveryNote::insertArticle(const QString &articleid,
                                 const QString &designation,
                                 const QString &quantity) {
  int row = m_articleTable->rows();
  m_articleTable->insertRows(row, 1);

  QTextCursor cursor = body->textCursor();
  QTextTableCell ce00 = m_articleTable->cellAt(row, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(articleid);

  QTextTableCell ce01 = m_articleTable->cellAt(row, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(designation);

  QTextTableCell ce02 = m_articleTable->cellAt(row, 2);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(quantity);

  body->document()->setModified(true);
}

bool DeliveryNote::generateDocument(QPrinter *printer) {
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

void DeliveryNote::generatePdf() {
  if (createPDF("deliverynotes")) {
    emit statusMessage(tr("PDF File written."));
  } else {
    emit statusMessage(tr("PDF not generated"));
  }
}

void DeliveryNote::openPrintDialog() {
  if (createPDF("deliverynotes")) {
    emit statusMessage(tr("PDF File written."));
  } else {
    emit statusMessage(tr("PDF not generated"));
  }

  if (p_printerName.isEmpty()) {
    emit statusMessage(tr("No Printer found"));
    return;
  }

  QPrinterInfo p_info = QPrinterInfo::printerInfo(p_printerName);
  QPrinter *printer = new QPrinter(p_info, QPrinter::ScreenResolution);
  printer->setResolution(72);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout());
  printer->setFullPage(true);
  printer->setDocName("DeliveryNote");
  printer->setPrinterName(p_printerName);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)),
          SLOT(generateDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

void DeliveryNote::setDelivery(int orderId, int customerId,
                               const QString &deliverNoteId) {
  if (orderId < 1) {
    warningMessageBox(tr("There is no Order-Id to generate this delivery!"));
    return;
  }
  p_orderId = QString::number(orderId).rightJustified(7, '0');

  if (customerId < 1) {
    warningMessageBox(tr("There is no Customer Id to generate this delivery!"));
    return;
  }
  p_customerId = QString::number(customerId).rightJustified(7, '0');

  if (deliverNoteId.isEmpty()) {
    warningMessageBox(tr("delivery note number is empty!"));
    return;
  }
  p_deliveryId = deliverNoteId;
}

int DeliveryNote::exec(const QList<BillingInfo> &list) {
  if (p_orderId < 1) {
    qFatal("you must call setDelivery() before exec!");
    return QDialog::Rejected;
  } else if (p_customerId < 1) {
    qFatal("you must call setDelivery() before exec!");
    return QDialog::Rejected;
  } else if (p_customerAddress.isEmpty()) {
    warningMessageBox(tr("<p>Customer Address is empty!</p>"));
    return QDialog::Rejected;
  }

  constructHeader();
  constructFooter();
  constructBody();

  QListIterator<BillingInfo> it(list);
  while (it.hasNext()) {
    BillingInfo d = it.next();
    insertArticle(d.articleid, d.designation, QString::number(d.quantity));
  }

  addPrinters();

  return Printing::exec();
}
