// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

#include <QLayout>
#include <QPrintDialog>

namespace AntiquaCRM
{

DeliveryNote::DeliveryNote(QWidget* parent) : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_deliverynote_page");
}

void DeliveryNote::setBodyLayout() {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(margin.left(), getPoints(100), margin.right(), margin.bottom());
  m_body = new APrintingBody(this);
  m_body->setFont(normalFont);
  layout->addWidget(m_body);
  setLayout(layout);

  QTextCursor cursor = m_body->textCursor();
  QTextTableFormat _tableFormat = m_body->tableFormat();
  _tableFormat.setBottomMargin(10);

  QTextTable* m_table = cursor.insertTable(1, 2, _tableFormat);
  QTextTableCell hcl = m_table->cellAt(0, 0);
  hcl.setFormat(m_body->tableCellFormat());
  hcl.firstCursorPosition().insertText(tr("Delivery note"));

  QTextTableCell hcr = m_table->cellAt(0, 1);
  hcr.setFormat(m_body->tableCellFormat());

  QString _dtext = companyData("COMPANY_LOCATION_NAME");
  _dtext.append(" " + tr("on") + " ");
  _dtext.append(QDate::currentDate().toString("dd.MM.yyyy"));

  cursor = hcr.firstCursorPosition();
  cursor.setBlockFormat(m_body->alignRight());
  cursor.beginEditBlock();
  cursor.insertText(_dtext);
  cursor.endEditBlock();
}

int DeliveryNote::addArticle(int row, const QSqlQuery& result) {
  if (m_articles == nullptr)
    return row;

  // Article
  QTextTableCell ce00 = m_articles->cellAt(row, 0);
  ce00.firstCursorPosition().insertText(result.value("aid").toString());

  // Title
  QTextTableCell ce01 = m_articles->cellAt(row, 1);
  ce01.firstCursorPosition().insertText(result.value("title").toString());

  // Quantity
  QTextTableCell ce02 = m_articles->cellAt(row, 2);
  QTextCursor cursor = ce02.firstCursorPosition();
  cursor.setBlockFormat(m_body->alignCenter());
  cursor.beginEditBlock();
  cursor.insertText(result.value("crowd").toString());
  cursor.endEditBlock();

  return row;
}

void DeliveryNote::paintContent(QPainter& painter) {
  Q_UNUSED(painter);
}

bool DeliveryNote::setContentData(QJsonObject& data) {
  setBodyLayout();
  if (!data.contains("config") || m_body == nullptr) {
    qWarning("Unable to read invoice delivery note data!");
    return false;
  }
  contentData = data;

  AntiquaCRM::ASqlFiles _tpl("query_printing_delivery_note");
  if (!_tpl.openTemplate()) {
    qWarning("Unable to open delivery note SQL template!");
  }

  m_body->insertText(companyData("COMPANY_DELIVERY_INTRO"));

  QTextCursor cursor = m_body->textCursor();
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
    // BEGIN::ArticleHeaderTable
    m_articles = cursor.insertTable(_size + 1, 3, m_body->tableFormat());
    m_articles->setObjectName("article_table");
    const QTextTableCellFormat _hcformat = m_body->headerTableCellFormat();
    // Article
    QTextTableCell ce00 = m_articles->cellAt(_row, 0);
    cursor = ce00.firstCursorPosition();
    ce00.setFormat(_hcformat);
    cursor.insertText(tr("Article"));
    // Description
    QTextTableCell ce01 = m_articles->cellAt(_row, 1);
    ce01.setFormat(_hcformat);
    ce01.firstCursorPosition().insertText(tr("Description"));
    // Quantity
    QTextTableCell ce02 = m_articles->cellAt(_row, 2);
    ce02.setFormat(_hcformat);
    cursor = ce02.firstCursorPosition();
    cursor.setBlockFormat(m_body->alignCenter());
    cursor.beginEditBlock();
    cursor.insertText(tr("Quantity"));
    cursor.endEditBlock();
    // END::ArticleHeaderTable

    // BEGIN::ArticleItems
    while (_query.next()) {
      addArticle(++_row, _query);
    }
    _query.clear();
    // END::ArticleItems
  }

  m_body->insertText(companyData("COMPANY_DELIVERY_THANKS"));
  return true;
}

PrintDeliveryNote::PrintDeliveryNote(QWidget* parent) : APrintDialog{parent} {
  setObjectName("print_deliverynote_dialog");
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(QMargins(0, 0, 0, 0));
  pageLayout.setUnits(QPageLayout::Millimeter);
  pageLayout.setMode(QPageLayout::FullPageMode);
}

void PrintDeliveryNote::renderPage(QPrinter* printer) {
  Q_CHECK_PTR(page);
  QPainter painter(printer);
  painter.setWindow(page->rect());
  painter.translate(0, 0);
  page->render(&painter);
  painter.end();
}

void PrintDeliveryNote::createPDF() {
  QDir _dir = config->getArchivPath(ANTIQUACRM_ARCHIVE_DELIVERY);
  if (_dir.exists()) {
    QFileInfo _file(_dir, pdfFileName);
    QPrinter* printer = new QPrinter(QPrinter::HighResolution);
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
  const QPrinterInfo printerInfo = getPrinterInfo();
  QPageLayout pageLayout = page->pageLayout();
  pageLayout.setMode(QPageLayout::FullPageMode);

  QPrinter* printer = new QPrinter(printerInfo, QPrinter::PrinterResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout);
  printer->setDocName("Invoice");
  printer->setPrinterName(printerInfo.printerName());
  QPrintDialog* dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter*)), SLOT(renderPage(QPrinter*)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
    sendStatusMessage(tr("Delivery note printed!"));
  }
}

int PrintDeliveryNote::exec(const QJsonObject& options, bool pdfbtn) {
  btn_pdf->setEnabled(pdfbtn);
  qint64 o_id = options.value("o_id").toInteger(0);
  if (o_id < 1) {
    qWarning("Missing Order ID.");
    return QDialog::Rejected;
  }

  qint64 c_id = options.value("o_customer_id").toInteger(0);
  if (c_id < 1) {
    qWarning("Missing Customer ID.");
    return QDialog::Rejected;
  }

  qint64 i_id = options.value("o_invoice_id").toInteger(0);
  if (i_id < 1) {
    qWarning("Missing Invoice ID.");
    return QDialog::Rejected;
  }

  // A Delivery number is generated with: {jear}{day of jear}{order id}
  // Thus the minimum length is (4+3+1)=8
  const QString d_id = options.value("o_delivery").toString();
  if (d_id.length() < 8) {
    qWarning("Missing Delivery Number %s.", qUtf8Printable(d_id));
    return QDialog::Rejected;
  }

  pdfFileName = d_id;
  pdfFileName.append(".pdf");

  QJsonObject _config;
  _config.insert("invoice_id", i_id);
  _config.insert("order_id", o_id);
  _config.insert("customer_id", c_id);
  _config.insert("delivery_id", d_id);

  page = new DeliveryNote(this);
  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);

  QJsonObject _content;
  _content.insert("config", _config);
  _content.insert("subject", tr("Delivery note"));
  _content.insert("address", _person.value("address").toString());

  QJsonArray _array;
  _array.append(tr("Invoice No."));
  _array.append(AntiquaCRM::AUtil::zerofill(i_id, 10));
  _content.insert("invoice_id", _array);

  _array = QJsonArray();
  _array.append(tr("Order No."));
  _array.append(AntiquaCRM::AUtil::zerofill(o_id, 10));
  _content.insert("order_id", _array);

  _array = QJsonArray();
  _array.append(tr("Customer No."));
  _array.append(AntiquaCRM::AUtil::zerofill(c_id, 10));
  _content.insert("customer_id", _array);

  _array = QJsonArray();
  _array.append(tr("Delivery No."));
  _array.append(d_id);
  _content.insert("delivery_id", _array);

  if (!page->setContentData(_content))
    return QDialog::Rejected;

  setPrintingPage(page);

  return QDialog::exec();
}

} // namespace AntiquaCRM
