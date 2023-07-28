// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printinvoice.h"

#include <QPrintDialog>

namespace AntiquaCRM {

InvoicePage::InvoicePage(QWidget *parent) : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_invoice_page");
}

void InvoicePage::setBody(qint64 oid, qint64 cid) {
  AntiquaCRM::ASqlFiles _tpl("query_printing_invoice");
  if (!_tpl.openTemplate() || oid < 1 || cid < 1) {
    qWarning("Unable to query invoice data!");
  }

  QString _sql("a_order_id=");
  _sql.append(QString::number(oid));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(cid));
  _sql = QString("a_order_id=481 AND a_customer_id=698");
  _tpl.setWhereClause(_sql);

  QTextCursor _cursor = textCursor();
  _cursor.insertBlock(bodyText());
  _cursor.beginEditBlock();
  QStringList _l;
  for (int i = 0; i < 50; ++i) { _l.append("text"); }
  _cursor.insertText(_l.join(" "));
  // _cursor.insertText(companyData("COMPANY_INTRO_DELIVERY"));
  _cursor.insertText("\n");
  _cursor.endEditBlock();

  double _full_price = 0.00;
  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  int _size = _query.size();
  if (_size > 0) {
    QFont _font = getFont("print_font_normal");
    QTextTable *m_tb = _cursor.insertTable((_size + 1), 4, inlineTableFormat());

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
    QTextTableCell t_c4 = m_tb->cellAt(_row, 3);
    t_c4.setFormat(charFormat(_font, true));
    t_c4.firstCursorPosition().insertText(tr("Price"));
    _row++;

    while (_query.next()) {
      QTextTableCell _tc1 = m_tb->cellAt(_row, 0);
      _cursor = _tc1.firstCursorPosition();
      _cursor.insertText(_query.value("a_article_id").toString());
      QTextTableCell _tc2 = m_tb->cellAt(_row, 1);
      _cursor = _tc2.firstCursorPosition();
      _cursor.insertText(_query.value("a_title").toString());
      QTextTableCell _tc3 = m_tb->cellAt(_row, 2);
      _cursor = _tc3.firstCursorPosition();
      _cursor.insertText(_query.value("a_count").toString());
      // price
      double _price = _query.value("a_sell_price").toDouble();
      _full_price += _price;
      QString _money = AntiquaCRM::ATaxCalculator::money(_price);
      QTextTableCell _tc4 = m_tb->cellAt(_row, 3);
      _cursor = _tc4.firstCursorPosition();
      _cursor.insertText(_money);
      _row++;
    }
  }
  _query.clear();

  qDebug() << Q_FUNC_INFO << "__TODO__" << oid << cid << _full_price;
}

PrintInvoice::PrintInvoice(QWidget *parent) : APrintDialog{parent} {
  setObjectName("print_invoice_dialog");
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(QMargins(0, 0, 0, 0));
  pageLayout.setUnits(QPageLayout::Millimeter);
  pageLayout.setMode(QPageLayout::FullPageMode);
}

void PrintInvoice::renderPage(QPrinter *printer) {
  Q_CHECK_PTR(page);
  QPainter painter(printer);
  painter.setWindow(page->rect());
  painter.translate(0, 0);
  page->render(&painter);
  painter.end();
}

void PrintInvoice::createPDF() {
  QDir _dir(config->value("dirs/archive_invoices").toString());
  if (_dir.exists()) {
    QFileInfo _file(_dir, pdfFileName);
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageLayout(pageLayout);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setCreator("AntiquaCRM");
    printer->setOutputFileName(_file.filePath());
    renderPage(printer);
    if (_file.isReadable())
      sendStatusMessage(tr("Invoice PDF created!"));
  }
}

void PrintInvoice::openPrintDialog() {
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
    sendStatusMessage(tr("Invoice printed!"));
  }
}

int PrintInvoice::exec(const QJsonObject &options) {
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

  qDebug() << Q_FUNC_INFO << options;

  pdfFileName = AntiquaCRM::AUtil::zerofill(o_id);
  pdfFileName.append(".pdf");

//  page = new InvoicePage(this);
//  // Company
//  QString _title = page->companyData("COMPANY_PRINTING_HEADER");
//  QString _company(page->companyData("COMPANY_SHORTNAME"));
//  _company.append(" - ");
//  _company.append(page->companyData("COMPANY_STREET"));
//  _company.append(" - ");
//  _company.append(page->companyData("COMPANY_LOCATION"));

//  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);
//  page->setRecipientData(_person.value("address").toString(), options);
//  setPrintingPage(page);

  return QDialog::exec();
}

} // namespace AntiquaCRM
