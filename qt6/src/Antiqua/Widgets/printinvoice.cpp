// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printinvoice.h"

#include <QLayout>
#include <QMetaType>
#include <QPrintDialog>
#include <QTableWidgetItem>

namespace AntiquaCRM {

InvoicePage::InvoicePage(QWidget *parent) : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_invoice_page");
  normalFont = getFont("print_font_normal");
}

void InvoicePage::paintContent(QPainter &painter) { Q_UNUSED(painter); }

void InvoicePage::setBodyLayout() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(margin.left(), getPoints(100), margin.right(),
                             margin.bottom());
  m_body = new APrintingBody(this);
  layout->addWidget(m_body);
  setLayout(layout);
}

bool InvoicePage::setContentData(QJsonObject &data) {
  setBodyLayout();
  if (!data.contains("config") || m_body == nullptr) {
    qWarning("Unable to read invoice content data!");
    return false;
  }
  contentData = data;

  QFont _header_font = getFont("print_font_normal");
  _header_font.setBold(true);

  QTextTableFormat _tableFormat = m_body->tableFormat();
  _tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  _tableFormat.setTopMargin(5);

  QTextCursor cursor = m_body->textCursor();
  cursor.setCharFormat(m_body->charFormat(_header_font));

  QTextTable *m_table = cursor.insertTable(1, 2, _tableFormat);
  m_table->setObjectName("article_table");

  QTextTableCell hcl = m_table->cellAt(0, 0);
  hcl.setFormat(m_body->cellFormat());
  hcl.firstCursorPosition().insertText(tr("Invoice"));

  QTextTableCell hcr = m_table->cellAt(0, 1);
  hcr.setFormat(m_body->cellFormat());
  QString _dtext = companyData("COMPANY_LOCATION_NAME");
  _dtext.append(" " + tr("on") + " ");
  _dtext.append(QDate::currentDate().toString("dd.MM.yyyy"));
  cursor = hcr.firstCursorPosition();
  cursor.setBlockFormat(m_body->alignRight());
  cursor.insertText(_dtext);

  AntiquaCRM::ASqlFiles _tpl("query_printing_invoice");
  if (!_tpl.openTemplate()) {
    qWarning("Unable to open invoice SQL template!");
  }

  QJsonObject _config = contentData.value("config").toObject();
  QString _sql("a_order_id=");
  _sql.append(QString::number(_config.value("order_id").toDouble()));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(_config.value("customer_id").toDouble()));
  _tpl.setWhereClause(_sql);

  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  int _size = _query.size();
  if (_size > 0) {
    qint8 _row = 0;
    const QFont _font = getFont("print_font_normal");
    cursor = m_body->textCursor();
    cursor.setCharFormat(m_body->charFormat(_font));

    m_table = cursor.insertTable(_size + 3, 4, _tableFormat);
    m_table->setObjectName("article_table");

    QTextTableCellFormat _cellFormat = m_body->cellFormat();
    _cellFormat.setTopBorder(1);
    _cellFormat.setTopBorderBrush(borderBrush());
    _cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    _cellFormat.setBottomBorder(1);
    _cellFormat.setBottomBorderBrush(borderBrush());
    _cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableCell ce00 = m_table->cellAt(_row, 0);
    ce00.setFormat(_cellFormat);
    cursor = ce00.firstCursorPosition();
    cursor.setBlockFormat(m_body->alignCenter());
    cursor.beginEditBlock();
    cursor.insertText(tr("Article"));
    cursor.endEditBlock();

    QTextTableCell ce01 = m_table->cellAt(_row, 1);
    ce01.setFormat(_cellFormat);
    ce01.firstCursorPosition().insertText(tr("Description"));

    QTextTableCell ce02 = m_table->cellAt(_row, 2);
    ce02.setFormat(_cellFormat);
    cursor = ce02.firstCursorPosition();
    cursor.setBlockFormat(m_body->alignCenter());
    cursor.beginEditBlock();
    cursor.insertText(tr("Quantity"));
    cursor.endEditBlock();

    QTextTableCell ce03 = m_table->cellAt(_row, 3);
    ce03.setFormat(_cellFormat);
    cursor = ce03.firstCursorPosition();
    cursor.setBlockFormat(m_body->alignCenter());
    cursor.beginEditBlock();
    cursor.insertText(tr("Price"));
    cursor.endEditBlock();

    while (_query.next()) {
      _row++;
      // Article
      qint64 _aid = _query.value("a_article_id").toLongLong();
      QTextTableCell ce00 = m_table->cellAt(_row, 0);
      ce00.firstCursorPosition().insertText(QString::number(_aid));
      // Title
      QTextTableCell ce01 = m_table->cellAt(_row, 1);
      ce01.firstCursorPosition().insertText(_query.value("a_title").toString());
      // Quantity
      int _count = _query.value("a_count").toInt();
      QTextTableCell ce02 = m_table->cellAt(_row, 2);
      cursor = ce02.firstCursorPosition();
      cursor.setBlockFormat(m_body->alignCenter());
      cursor.beginEditBlock();
      cursor.insertText(QString::number(_count));
      cursor.endEditBlock();
      // Price
      double _price = _query.value("a_sell_price").toDouble();
      int _vat_type = _query.value("a_tax").toInt();
      AntiquaCRM::ATaxCalculator _calc(_price, _vat_type);
      QTextTableCell ce03 = m_table->cellAt(_row, 3);
      cursor = ce03.firstCursorPosition();
      cursor.setBlockFormat(m_body->alignRight());
      cursor.beginEditBlock();
      cursor.insertText(_calc.money(_price));
      cursor.endEditBlock();

      // qDebug() << _aid << _count << _calc.money(_price);
    }
    _query.clear();
  }

  // companyData("COMPANY_INVOICE_INTRO");
  // companyData("COMPANY_INVOICE_THANKS");

  return true;
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
  const QPrinterInfo printerInfo = getPrinterInfo();
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

  double _iid = options.value("invoice_id").toDouble();
  double _did = options.value("order_id").toDouble();

  pdfFileName = AntiquaCRM::AUtil::zerofill(o_id);
  pdfFileName.append(".pdf");

  page = new InvoicePage(this);

  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);

  QJsonObject _config;
  _config.insert("invoice_id", _iid);
  _config.insert("order_id", o_id);
  _config.insert("customer_id", c_id);
  _config.insert("delivery_id", _did);

  QJsonObject content;
  content.insert("config", _config);
  content.insert("subject", tr("Invoice"));
  content.insert("address", _person.value("address").toString());

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
