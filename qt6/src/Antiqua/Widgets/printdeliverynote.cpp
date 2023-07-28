// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

#include <QPrintDialog>

namespace AntiquaCRM {

DeliveryNote::DeliveryNote(QWidget *parent)
    : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_deliverynote_page");
}

void DeliveryNote::setBody(qint64 oid, qint64 cid) {
  AntiquaCRM::ASqlFiles _tpl("query_printing_delivery_note");
  if (!_tpl.openTemplate() || oid < 1 || cid < 1) {
    qWarning("Unable to query delivery note data!");
  }

  QString _sql("a_order_id=");
  _sql.append(QString::number(oid));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(cid));
  _tpl.setWhereClause(_sql);

  QTextCursor _cursor = textCursor();
  _cursor.insertBlock(bodyText());
  _cursor.beginEditBlock();
  _cursor.insertText(companyData("COMPANY_INTRO_DELIVERY"));
  _cursor.insertText("\n");
  _cursor.endEditBlock();

  // BEGIN:Table
  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  int _size = _query.size();
  if (_size > 0) {
    QFont _font = getFont("print_font_normal");
    QTextTable *m_tb = _cursor.insertTable((_size + 1), 3, tableFormat());
    m_tb->setObjectName("DELIVERY_TABLE");

    int _row = 0;
    QTextTableCell t_c1 = m_tb->cellAt(_row, 0);
    t_c1.setFormat(charFormat(_font, true));
    t_c1.firstCursorPosition().insertText(tr("Article"));
    QTextTableCell t_c2 = m_tb->cellAt(_row, 1);
    t_c2.setFormat(charFormat(_font, true));
    t_c2.firstCursorPosition().insertText(tr("Product"));
    QTextTableCell t_c3 = m_tb->cellAt(_row, 2);
    t_c3.setFormat(charFormat(_font, true));
    t_c3.firstCursorPosition().insertText(tr("Quantity"));
    _row++;

    while (_query.next()) {
      QTextTableCell _tc1 = m_tb->cellAt(_row, 0);
      _tc1.firstCursorPosition().insertText(_query.value("aid").toString());
      QTextTableCell _tc2 = m_tb->cellAt(_row, 1);
      _tc2.firstCursorPosition().insertText(_query.value("title").toString());
      QTextTableCell _tc3 = m_tb->cellAt(_row, 2);
      _tc3.setFormat(verticalCenter());
      _cursor = _tc3.firstCursorPosition();
      _cursor.setBlockFormat(alignCenter());
      _cursor.beginEditBlock();
      _cursor.insertText(_query.value("crowd").toString().trimmed());
      _cursor.endEditBlock();
      _row++;
    }
  }
  _query.clear();
  // END:Table

  // Comments
  _cursor = textCursor();
  _cursor.insertBlock(bodyText());
  _cursor.beginEditBlock();
  _cursor.insertText(companyData("COMPANY_INVOICE_THANKS"));
  _cursor.endEditBlock();

  QTextBlockFormat _thanks = bodyText();
  _thanks.setIndent(1);
  _cursor.insertBlock(_thanks);
  _cursor.beginEditBlock();
  _cursor.insertText(companyData("COMPANY_EMPLOYER"));
  _cursor.endEditBlock();
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

  pdfFileName = AntiquaCRM::AUtil::zerofill(o_id);
  pdfFileName.append(".pdf");

  QJsonObject content;
  content.insert("subject", tr("Delivery note"));

  page = new DeliveryNote(this);

  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);
  content.insert("address", _person.value("address").toString());

  double _iid = options.value("invoice_id").toDouble();
  double _did = options.value("order_id").toDouble();
  QJsonArray _array;
  _array.append(tr("Invoice No."));
  _array.append(AntiquaCRM::AUtil::zerofill(_iid, 10));
  content.insert("invoice_id", _array);

  _array = QJsonArray();
  _array.append(tr("Order No."));
  _array.append(AntiquaCRM::AUtil::zerofill(o_id, 10));
  content.insert("order_id", _array);

  _array = QJsonArray();
  _array.append(tr("Costumer No."));
  _array.append(AntiquaCRM::AUtil::zerofill(c_id, 10));
  content.insert("customer_id", _array);

  _array = QJsonArray();
  _array.append(tr("Delivery No."));
  _array.append(AntiquaCRM::AUtil::zerofill(_did, 10));
  content.insert("delivery_id", _array);

  if (!page->setContentData(content))
    return QDialog::Rejected;

  setPrintingPage(page);

  return QDialog::exec();
}

} // namespace AntiquaCRM
