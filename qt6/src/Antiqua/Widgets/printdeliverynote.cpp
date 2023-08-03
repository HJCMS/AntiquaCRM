// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

#include <QLayout>
#include <QPrintDialog>

namespace AntiquaCRM {

DeliveryNote::DeliveryNote(QWidget *parent)
    : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_deliverynote_page");
  normalFont = getFont("print_font_normal");
}

void DeliveryNote::paintContent(QPainter &painter) { Q_UNUSED(painter); }

void DeliveryNote::setBodyLayout() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(margin.left(), getPoints(100), margin.right(),
                             margin.bottom());
  m_body = new APrintingBody(this);
  layout->addWidget(m_body);
  setLayout(layout);
}

bool DeliveryNote::setContentData(QJsonObject &data) {
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

  QJsonObject _config = contentData.value("config").toObject();
  QString _sql("a_order_id=");
  _sql.append(QString::number(_config.value("order_id").toDouble()));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(_config.value("customer_id").toDouble()));
  _tpl.setWhereClause(_sql);

  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  int _size = _query.size();
  if (_size > 0) {
    const QFont _font = getFont("print_font_normal");
    const QTextCharFormat _charFormat = m_body->charFormat(_font);

    QTextCursor cursor = m_body->textCursor();
    cursor.setCharFormat(_charFormat);

    QTextTableFormat _tableFormat = m_body->tableFormat();
    _tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
    _tableFormat.setTopMargin(5);

    QTextTable *m_table = cursor.insertTable(_size + 1, 3, _tableFormat);
    m_table->setObjectName("article_table");

    QTextTableCellFormat _cellFormat = m_body->cellFormat();
    _cellFormat.setTopBorder(1);
    _cellFormat.setTopBorderBrush(borderBrush());
    _cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    _cellFormat.setBottomBorder(1);
    _cellFormat.setBottomBorderBrush(borderBrush());
    _cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    qint8 _row = 0;
    QTextTableCell ce00 = m_table->cellAt(_row, 0);
    cursor = ce00.firstCursorPosition();
    ce00.setFormat(_cellFormat);
    cursor.insertText(tr("Article-ID"));

    QTextTableCell ce01 = m_table->cellAt(_row, 1);
    ce01.setFormat(_cellFormat);
    ce01.firstCursorPosition().insertText(tr("Designation"));

    QTextTableCell ce02 = m_table->cellAt(_row, 2);
    ce02.setFormat(_cellFormat);
    ce02.firstCursorPosition().insertText(tr("Quantity"));

    while (_query.next()) {
      _row++;
      // Article
      QTextTableCell ce00 = m_table->cellAt(_row, 0);
      ce00.firstCursorPosition().insertText(_query.value("aid").toString());
      // Title
      QTextTableCell ce01 = m_table->cellAt(_row, 1);
      ce01.firstCursorPosition().insertText(_query.value("title").toString());
      // Quantity
      QTextTableCell ce02 = m_table->cellAt(_row, 2);
      ce02.firstCursorPosition().insertText(_query.value("crowd").toString());
    }
    _query.clear();

    cursor = m_body->textCursor();
    cursor.setCharFormat(_charFormat);
    cursor.setBlockFormat(m_body->alignLeft());
    cursor.beginEditBlock();
    cursor.insertText("\n\n");
    cursor.insertText(companyData("COMPANY_DELIVERY_THANKS"));
    cursor.endEditBlock();
  }

  return true;
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

  page = new DeliveryNote(this);
  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);

  double _iid = options.value("invoice_id").toDouble();
  double _did = options.value("order_id").toDouble();

  QJsonObject _config;
  _config.insert("invoice_id", _iid);
  _config.insert("order_id", o_id);
  _config.insert("customer_id", c_id);
  _config.insert("delivery_id", _did);

  QJsonObject content;
  content.insert("config", _config);
  content.insert("subject", tr("Delivery note"));
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
