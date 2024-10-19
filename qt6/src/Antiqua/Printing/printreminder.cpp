// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printreminder.h"

#include <QLayout>
#include <QMetaType>
#include <QPrintDialog>
#include <QTableWidgetItem>

namespace AntiquaCRM {

ReminderPage::ReminderPage(QWidget *parent)
    : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_reminder_page");
}

void ReminderPage::paintContent(QPainter &painter) { Q_UNUSED(painter); }

void ReminderPage::setBodyLayout() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(margin.left(), getPoints(95), margin.right(),
                             margin.bottom());
  m_body = new APrintingBody(this);
  m_body->setFont(normalFont);
  layout->addWidget(m_body);
  setLayout(layout);

  QTextCursor cursor = m_body->textCursor();
  QTextTableFormat _tableFormat = m_body->tableFormat();
  _tableFormat.setBottomMargin(10);

  QTextTable *m_table = cursor.insertTable(1, 2, _tableFormat);
  m_table->setObjectName("header_table");

  QTextTableCell hcl = m_table->cellAt(0, 0);
  hcl.setFormat(m_body->tableCellFormat());
  m_body->setCellItem(hcl, tr("Reminder"), Qt::AlignLeft);

  QTextTableCell hcr = m_table->cellAt(0, 1);
  hcr.setFormat(m_body->tableCellFormat());
  QString _dtext = companyData("COMPANY_LOCATION_NAME");
  _dtext.append(" " + tr("on") + " ");
  _dtext.append(QDate::currentDate().toString("dd.MM.yyyy"));
  m_body->setCellItem(hcr, _dtext, Qt::AlignRight);
}

void ReminderPage::setArticleCell(int row, const QVariant &value) {
  QTextTableCell _tc = m_articles->cellAt(row, 0);
  _tc.setFormat(m_body->articleTableCellFormat((row > 1)));
  m_body->setCellItem(_tc, value.toLongLong(),
                      (Qt::AlignRight | Qt::AlignVCenter));
}

void ReminderPage::setDescripeCell(int row, const QVariant &value) {
  QTextTableCell _tc = m_articles->cellAt(row, 1);
  _tc.setFormat(m_body->articleTableCellFormat((row > 1)));
  m_body->setCellItem(_tc, value.toString(), (Qt::AlignLeft | Qt::AlignTop));
}

void ReminderPage::setQuantityCell(int row, const QVariant &value) {
  QTextTableCell _tc = m_articles->cellAt(row, 2);
  _tc.setFormat(m_body->articleTableCellFormat((row > 1)));
  m_body->setCellItem(_tc, value.toLongLong(),
                      (Qt::AlignCenter | Qt::AlignVCenter));
}

int ReminderPage::addArticleRows(int row, const QSqlQuery &result) {
  if (m_articles == nullptr)
    return row;

  int _row = row;
  // Article
  setArticleCell(_row, result.value("a_article_id"));
  // Title
  setDescripeCell(_row, result.value("a_title"));
  // Quantity
  int _count = result.value("a_count").toInt();
  setQuantityCell(_row, _count);

  // Price
  QString _vat_prefix;
  double _price = result.value("a_sell_price").toDouble();
  AntiquaCRM::ATaxCalculator _calc(_price, result.value("a_tax").toInt());
  _calc.setBillingMode(config.value("vat_level").toInt());

  switch (_calc.getBillingMode()) {
  case TAX_INCL:
    _vat_prefix = tr("incl."); // including sales tax
    break;

  case TAX_WITH:
    _vat_prefix = tr("with"); // with sales tax
    break;

  default:
    _vat_prefix = tr("without"); // without sales tax
    break;
  }

  QTextTableCell _price_cell = m_articles->cellAt(_row, 3);
  _price_cell.setFormat(m_body->articleTableCellFormat((_row > 1)));
  m_body->setCellItem(_price_cell, _calc.money(_price),
                      (Qt::AlignRight | Qt::AlignVCenter));

  _row++;
  // m_articles->mergeCells(row, 0, 0, 3);
  QTextTableCell _vat_cell = m_articles->cellAt(_row, 2);
  _vat_cell.setFormat(m_body->articleTableCellFormat());

  QTextTableCell _subtotal_cell = m_articles->cellAt(_row, 3);
  _subtotal_cell.setFormat(m_body->articleTableCellFormat(true));

  int _type = result.value("a_type").toInt();
  switch (static_cast<AntiquaCRM::ArticleType>(_type)) {
  case (AntiquaCRM::ArticleType::BOOK): {
    QString _str(_vat_prefix);
    _str.append(" ");
    if (_calc.salesTaxRate() > 0) {
      _str.append(QString::number(_calc.salesTaxRate()));
      _str.append("% ");
    }
    _str.append(tr("VAT"));
    m_body->setCellItem(_vat_cell, _str, (Qt::AlignRight | Qt::AlignVCenter));
    m_body->setCellItem(_subtotal_cell, _calc.money(_calc.vatCosts()),
                        (Qt::AlignRight | Qt::AlignVCenter));
  } break;

  default: {
    QString _str(_vat_prefix);
    _str.append(" ");
    if (_calc.salesTaxRate() > 0) {
      _str.append(QString::number(_calc.salesTaxRate()));
      _str.append("% ");
    }
    _str.append(tr("VAT"));
    m_body->setCellItem(_vat_cell, _str, (Qt::AlignRight | Qt::AlignVCenter));
    m_body->setCellItem(_subtotal_cell, _calc.money(_calc.vatCosts()),
                        (Qt::AlignRight | Qt::AlignVCenter));
  } break;
  }
  summary += _calc.salesPrice();
  // qDebug() << "Summary" << summary;
  return _row;
}

bool ReminderPage::setContentData(QJsonObject &data) {
  setBodyLayout();
  if (!data.contains("config") || m_body == nullptr) {
    qWarning("Unable to read Reminder content data!");
    return false;
  }
  contentData = data;
  config = contentData.value("config").toObject();

  m_body->insertText(companyData("COMPANY_PAYMENT_REMINDER_OPEN"));

  AntiquaCRM::ASqlFiles _tpl("query_printing_invoice");
  if (!_tpl.openTemplate()) {
    qWarning("Unable to open Reminder SQL template!");
  }

  QString _sql("a_order_id=");
  _sql.append(QString::number(config.value("order_id").toDouble()));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(config.value("customer_id").toDouble()));
  _tpl.setWhereClause(_sql);

  QTextCursor _cursor = m_body->textCursor();
  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  qsizetype _size = _query.size();
  if (_size > 0) {
    // BEGIN::ArticleHeaderTable
    _cursor = m_body->textCursor();
    // querySize * (2 article rows) + HeaderRow
    int _rows = ((_size * 2) + 1);
    m_articles = _cursor.insertTable(_rows, 4, m_body->tableFormat());
    m_articles->setObjectName("article_table");

    QTextTableCellFormat _headerFormat = m_body->headerTableCellFormat();

    QTextTableCell ce00 = m_articles->cellAt(0, 0);
    ce00.setFormat(_headerFormat);
    m_body->setCellItem(ce00, tr("Article"), Qt::AlignCenter);

    QTextTableCell ce01 = m_articles->cellAt(0, 1);
    ce01.setFormat(_headerFormat);
    m_body->setCellItem(ce01, tr("Description"), Qt::AlignLeft);

    QTextTableCell ce02 = m_articles->cellAt(0, 2);
    ce02.setFormat(_headerFormat);
    m_body->setCellItem(ce02, tr("Quantity"), Qt::AlignCenter);

    QTextTableCell ce03 = m_articles->cellAt(0, 3);
    ce03.setFormat(_headerFormat);
    m_body->setCellItem(ce03, tr("Price"), Qt::AlignCenter);
    // END::ArticleHeaderTable

    int _row = 1;
    while (_query.next()) {
      _row = addArticleRows(_row, _query);
      _row++;
    }
    _query.clear();
  }

  // Summary - Delivery - Total
  int _row = m_articles->rows();
  m_articles->appendRows(3);
  m_articles->mergeCells(_row, 0, 1, 3);
  // Summary Subject
  QTextTableCell _st0 = m_articles->cellAt(_row, 2);
  _st0.setFormat(m_body->articleTableCellFormat(true));
  m_body->setCellItem(_st0, tr("subtotal"), Qt::AlignRight);
  // Subtotal Price
  QTextTableCell _st1 = m_articles->cellAt(_row, 3);
  _st1.setFormat(m_body->articleTableCellFormat(true));
  m_body->setCellItem(_st1, AntiquaCRM::ATaxCalculator::money(summary),
                      Qt::AlignRight);

  _row++;
  double delivery_cost = config.value("package_price").toDouble();
  // Delivery cost summary
  QTextTableCell _dc0 = m_articles->cellAt(_row, 2);
  _dc0.setFormat(m_body->articleTableCellFormat(false));
  m_body->setCellItem(_dc0, tr("delivery cost"), Qt::AlignRight);
  // Delivery cost
  QTextTableCell _dc1 = m_articles->cellAt(_row, 3);
  _dc1.setFormat(m_body->articleTableCellFormat(true));
  m_body->setCellItem(_dc1, AntiquaCRM::ATaxCalculator::money(delivery_cost),
                      Qt::AlignRight);

  _row++;
  // Total
  QTextTableCell _tp0 = m_articles->cellAt(_row, 2);
  _tp0.setFormat(m_body->articleTableCellFormat(false));
  m_body->setCellItem(_tp0, tr("total"), Qt::AlignRight);
  // Total
  summary += delivery_cost;
  QTextTableCell _tp1 = m_articles->cellAt(_row, 3);
  _tp1.setFormat(m_body->articleTableCellFormat(true));
  m_body->setCellItem(_tp1, AntiquaCRM::ATaxCalculator::money(summary),
                      Qt::AlignRight);

  m_body->insertText(companyData("COMPANY_PAYMENT_REMINDER_FINAL"));

  return true;
}

PrintReminder::PrintReminder(QWidget *parent) : APrintDialog{parent} {
  setObjectName("print_Reminder_dialog");
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(QMargins(0, 0, 0, 0));
  pageLayout.setUnits(QPageLayout::Millimeter);
  pageLayout.setMode(QPageLayout::FullPageMode);
}

void PrintReminder::renderPage(QPrinter *printer) {
  Q_CHECK_PTR(page);
  // Bug Windows lost pageLayout
  if (!printer->pageLayout().isValid())
    printer->setPageLayout(page->pageLayout());

  QPainter _painter(printer);
  _painter.setWindow(page->rect());
  page->render(&_painter);
  _painter.end();
}

void PrintReminder::createPDF() {
  QDir _dir = config->getArchivPath(ANTIQUACRM_ARCHIVE_REMINDERS);
  if (_dir.exists()) {
    QFileInfo _file(_dir, pdfFileName);
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageLayout(page->pageLayout());
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setPdfVersion(QPagedPaintDevice::PdfVersion_1_6);
    printer->setCreator(ANTIQUACRM_USERAGENT);
    printer->setFullPage(true);
    printer->setOutputFileName(_file.filePath());
    renderPage(printer);
    if (_file.isReadable())
      sendStatusMessage(tr("Reminder PDF created!"));
  }
}

void PrintReminder::openPrintDialog() {
  const QPrinterInfo printerInfo = getPrinterInfo();
  QPageLayout pageLayout = page->pageLayout();
  pageLayout.setMode(QPageLayout::FullPageMode);

  QPrinter *printer = new QPrinter(printerInfo, QPrinter::PrinterResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout);
  printer->setDocName("Reminder");
  printer->setPrinterName(printerInfo.printerName());
  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)), SLOT(renderPage(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
    sendStatusMessage(tr("Reminder printed!"));
  }
}

int PrintReminder::exec(const QJsonObject &options, bool pdfbtn) {
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

  pdfFileName = AntiquaCRM::AUtil::zerofill(o_id);
  pdfFileName.append(".pdf");

  page = new ReminderPage(this);

  QMap<QString, QVariant> _person = page->queryCustomerData(c_id);

  QJsonObject _config;
  _config.insert("invoice_id", i_id);
  _config.insert("order_id", o_id);
  _config.insert("customer_id", c_id);
  _config.insert("delivery_id", d_id);
  _config.insert("vat_level", options.value("o_vat_levels").toInt());
  int _status = options.value("o_payment_status").toInt();
  if (static_cast<AntiquaCRM::OrderPayment>(_status) ==
      AntiquaCRM::OrderPayment::NOTPAID) {
    _config.insert("payment_status", false);
  } else {
    _config.insert("payment_status", true);
  }
  _config.insert("package_price", options.value("package_price").toDouble());

  QJsonObject _content;
  _content.insert("config", _config);
  _content.insert("subject", tr("Invoice"));
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
  adjustSize();

  return QDialog::exec();
}

} // namespace AntiquaCRM
