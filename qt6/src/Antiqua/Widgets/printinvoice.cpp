// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printinvoice.h"

namespace AntiquaCRM {

InvoicePage::InvoicePage(QWidget *parent) : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_invoice_page");
}

const QPageLayout InvoicePage::pageLayout() const {
  QPageLayout _layout;
  _layout.setOrientation(QPageLayout::Portrait);
  _layout.setPageSize(QPageSize(QPageSize::A4));
  _layout.setMinimumMargins(QMargins(0, 0, 0, 0));
  _layout.setMargins(QMargins(marginLeft, 0, marginRight, 0));
  return _layout;
}

void InvoicePage::setLetterSubject(const QString &text) {
  QTextBlockFormat _block;
  _block.setAlignment(Qt::AlignLeft);
  QTextCursor _cursor = mainFrame->lastCursorPosition();
  QFont _font = getFont("print_font_subject");
  _cursor.setCharFormat(charFormat(_font, true));
  _cursor.insertBlock(_block);
  _cursor.insertText(text);
  _cursor.atEnd();
}

PrintInvoice::PrintInvoice(QWidget *parent) : APrintDialog{parent} {}

bool PrintInvoice::generateDocument(QPrinter *printer) {
  Q_UNUSED(printer);
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
}

void PrintInvoice::openPrintDialog() { qDebug() << Q_FUNC_INFO << "TODO"; }

int PrintInvoice::exec(const QJsonObject &options) {
  Q_UNUSED(options);

  page = new InvoicePage(this);
  page->setLetterHeading();

  qint64 invoiceId = 4;
  qint64 orderId = 45;
  qint64 customerId = 381;

  QMap<QString, QVariant> _person = page->queryCustomerData(customerId);
  QTextTable *table = page->recipientAddress(tr("Invoice"));
  int row = (table->rows() - 1);

  // Anschrift
  QTextCursor cursor = page->textCursor();
  QTextTableCell addrCell = table->cellAt(row, 0);
  addrCell.setFormat(page->addressCellFormat());
  cursor = addrCell.firstCursorPosition();
  int lines = 0;
  foreach (QString _l, _person.value("address").toString().split("\n")) {
    cursor.insertText(_l);
    cursor.insertText("\n");
    lines++;
  }
  // 4 Address lines required
  while (lines < 4) {
    cursor.insertText("\n");
    lines++;
  }

  // Betreff Informationen
  QMap<qint8, QString> title;
  title.insert(0, tr("Invoice No."));
  title.insert(1, tr("Order No."));
  title.insert(2, tr("Costumer No."));

  QMap<qint8, QString> data;
  data.insert(0, AntiquaCRM::AUtil::zerofill(invoiceId));
  data.insert(1, AntiquaCRM::AUtil::zerofill(orderId));
  data.insert(2, AntiquaCRM::AUtil::zerofill(customerId));

  QFont _font = page->getFont("print_font_normal");
  QTextTableCell infoCell = table->cellAt(row, 1);
  infoCell.setFormat(page->charFormat(_font));
  cursor = infoCell.firstCursorPosition();

  QTextTableFormat _tbformat = page->inlineTableFormat();
  QTextTable *child_table = cursor.insertTable(data.size(), 3, _tbformat);
  QMapIterator<qint8, QString> it(data);
  while (it.hasNext()) {
    it.next();
    // left
    QTextTableCell cl = child_table->cellAt(it.key(), 0);
    cl.setFormat(page->charFormat(_font));
    cursor = cl.firstCursorPosition();
    cursor.setBlockFormat(page->alignRight());
    cursor.insertText(title[it.key()]);
    // middle
    QTextTableCell cm = child_table->cellAt(it.key(), 1);
    cm.setFormat(page->charFormat(_font));
    cursor = cm.firstCursorPosition();
    cursor.setBlockFormat(page->alignCenter());
    cursor.insertText(":");
    // right
    QTextTableCell cr = child_table->cellAt(it.key(), 2);
    cr.setFormat(page->charFormat(_font));
    cursor = cr.firstCursorPosition();
    cursor.setBlockFormat(page->alignRight());
    cursor.insertText(it.value());
  }

  _font = page->getFont("print_font_subject");
  cursor = page->textCursor();
  QTextTable *subject_table = cursor.insertTable(1, 2, _tbformat);
  QTextTableCell scl0 = subject_table->cellAt(0, 0);
  scl0.setFormat(page->charFormat(_font));
  cursor = scl0.firstCursorPosition();
  cursor.insertText(tr("Invoice"));

  QTextTableCell scl1 = subject_table->cellAt(0, 1);
  scl1.setFormat(page->charFormat(_font));
  cursor = scl1.firstCursorPosition();
  cursor.setBlockFormat(page->alignRight());
  cursor.insertText(QDate::currentDate().toString("dd.MM.yyyy"));

  setPrintingPage(page);

  return QDialog::exec();
}

} // namespace AntiquaCRM
