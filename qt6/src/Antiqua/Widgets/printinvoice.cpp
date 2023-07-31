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
  // https://de.wikipedia.org/wiki/DIN_5008
  normalFont = getFont("print_font_normal");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(borderLeft(),              // left margin
                             getPoints(120),            // top margin
                             (width() - borderRight()), // right margin
                             getPoints(100));

  QString _css("* {background-color:#FFFFFF;color:#000000;border:none;}");

  m_table = new QTableWidget(this);
  m_table->setColumnCount(5);
  m_table->setStyleSheet(_css);
  m_table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  m_table->setSelectionMode(QAbstractItemView::NoSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->setWordWrap(true);

  setArticleHeaderItem(0, tr("Article"), (Qt::AlignCenter));
  setArticleHeaderItem(1, tr("Description"), (Qt::AlignLeft));
  setArticleHeaderItem(2, tr("Amount"), (Qt::AlignCenter));
  setArticleHeaderItem(3, tr("VAT"), (Qt::AlignCenter));
  setArticleHeaderItem(4, tr("Price"), (Qt::AlignCenter));

  QHeaderView *m_hview = m_table->horizontalHeader();
  m_hview->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_hview->setSectionResizeMode(1, QHeaderView::Stretch);

  QHeaderView *m_vview = m_table->verticalHeader();
  m_vview->setVisible(false);
  m_vview->setSectionResizeMode(QHeaderView::ResizeToContents);

  layout->addWidget(m_table);
  setLayout(layout);
}

const QPoint InvoicePage::startPoint() const {
  return QPoint(borderLeft(), getPoints(100));
}

void InvoicePage::setArticleData(int row, int column, const QVariant &data) {
  // Table Rows
  QTableWidgetItem *item = new QTableWidgetItem(data.toString());
  switch (column) {
  case 0:
    item->setTextAlignment(Qt::AlignRight);
    break;

  case 1:
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    break;

  default:
    item->setTextAlignment(Qt::AlignCenter);
    break;
  }
  m_table->setItem(row, column, item);
}

void InvoicePage::setArticleVAT(int row, int column, int type) {
  QString _title;
  switch (type) {
  case 0:
    _title = QString("19%");
    break;

  case 1:
    _title = QString("7%");
    break;

  default:
    _title = QString(" - ");
    break;
  }
  QTableWidgetItem *item = new QTableWidgetItem(_title);
  item->setData(Qt::UserRole, type);
  item->setTextAlignment(Qt::AlignCenter);
  m_table->setItem(row, column, item);
}

int InvoicePage::setArticlePrice(int row, int column, double price) {
  AntiquaCRM::ATaxCalculator _calc(price);
  QTableWidgetItem *item = new QTableWidgetItem(_calc.money(price));
  item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  item->setData(Qt::UserRole, price);
  m_table->setItem(row, column, item);

  row++;
  return row;
}

void InvoicePage::setArticleSummary() {
  int _row = (m_table->rowCount() - 1);
  int _vat = m_table->item(_row, 3)->data(Qt::UserRole).toInt();
  double _price = m_table->item(_row, 4)->data(Qt::UserRole).toDouble();
  switch (static_cast<AntiquaCRM::SalesTax>(_vat)) {
  case (AntiquaCRM::SalesTax::TAX_NOT):
    // without sales tax
    break;

  case (AntiquaCRM::SalesTax::TAX_INCL):
    // including sales tax
    break;

  case (AntiquaCRM::SalesTax::TAX_WITH):
    // with sales tax
    break;

  default:
    // unknown sales tax
    break;
  }
  qDebug() << Q_FUNC_INFO << "__TODO__" << _vat << _price;
}

void InvoicePage::paintSubject(QPainter &painter) {
  painter.setPen(fontPen());
  QFont _subjectFont(normalFont);
  _subjectFont.setBold(true);
  painter.setFont(_subjectFont);

  // Subject
  QStaticText _subject = textBlock();
  _subject.setText(contentData.value("subject").toString());
  _subject.prepare(painter.transform(), _subjectFont);
  painter.drawStaticText(startPoint(), _subject);

  // Location and Date
  painter.setFont(normalFont);
  QString _location(companyData("COMPANY_LOCATION_NAME"));
  _location.append(" " + tr("on") + " ");
  _location.append(QDate::currentDate().toString("dd.MM.yyyy"));
  QStaticText _datebox = textBlock(Qt::AlignRight);
  _datebox.setText(_location);
  _datebox.prepare(painter.transform(), normalFont);
  qreal _bx = (borderRight() - _datebox.size().width());
  painter.drawStaticText(QPoint(_bx, startPoint().y()), _datebox);

  painter.setPen(linePen());
  qreal _ly = (startPoint().y() + fontHeight(normalFont));
  painter.drawLine(QPoint(borderLeft(), _ly), QPoint(borderRight(), _ly));

  position = _ly;
}

void InvoicePage::paintContent(QPainter &painter) {
  if (!contentData.contains("body"))
    return; // skip if not exists

  // at first paint subject and date with underline.
  paintSubject(painter);

  // dataset
  const QJsonObject _body = contentData.value("body").toObject();

  // config
  qreal _fontHeight = fontHeight(normalFont);
  qreal _margin = 5;
  qreal _y = position + getPoints(_margin);
  qreal _x = borderLeft();
  QTransform _transform = painter.transform();

  // init painter
  painter.setPen(fontPen());
  painter.setFont(normalFont);

  if (_body.contains("intro")) {
    QStaticText _intro = textBlock();
    _intro.setText(_body.value("intro").toString());
    _intro.prepare(_transform, normalFont);
    _intro.setTextWidth(inlineWidth());
    painter.drawStaticText(QPoint(_x, _y), _intro);
  }

  _y += m_table->rect().bottomLeft().y() + getPoints(_margin) + _fontHeight;

  if (_body.contains("finally")) {
    QStaticText _finally = textBlock();
    _finally.setText(_body.value("finally").toString());
    _finally.prepare(_transform, normalFont);
    _finally.setTextWidth(inlineWidth());
    painter.drawStaticText(QPoint(_x, _y), _finally);
  }
}

bool InvoicePage::setContentData(QJsonObject &data) {
  if (!data.contains("config")) {
    qWarning("Unable to read invoice content data!");
    return false;
  }

  contentData = data;
  QJsonObject _config = contentData.value("config").toObject();
  QJsonObject _body;
  _body.insert("intro", companyData("COMPANY_INVOICE_INTRO"));
  _body.insert("finally", companyData("COMPANY_INVOICE_THANKS"));
  contentData.insert("body", _body);

  AntiquaCRM::ASqlFiles _tpl("query_printing_invoice");
  if (!_tpl.openTemplate()) {
    qWarning("Unable to open invoice SQL template!");
  }

  QString _sql("a_order_id=");
  _sql.append(QString::number(_config.value("order_id").toDouble()));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(_config.value("customer_id").toDouble()));
  _tpl.setWhereClause(_sql);

  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  int _size = _query.size();
  if (_size > 0) {
    m_table->setRowCount(_query.size());
    int row = 0;
    while (_query.next()) {
      setArticleData(row, 0, _query.value("a_article_id").toString());
      setArticleData(row, 1, _query.value("a_title").toString());
      setArticleData(row, 2, _query.value("a_count").toInt());
      setArticleVAT(row, 3, _query.value("a_tax").toInt());
      row = setArticlePrice(row, 4, _query.value("a_sell_price").toDouble());
    }
    setArticleSummary();
    _query.clear();
  }
  return (m_table->rowCount() > 0);
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
