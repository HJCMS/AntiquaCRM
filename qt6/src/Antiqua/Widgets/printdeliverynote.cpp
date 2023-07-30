// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QPrintDialog>
#include <QSizePolicy>

namespace AntiquaCRM {

DeliveryNote::DeliveryNote(QWidget *parent)
    : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_deliverynote_page");
  // https://de.wikipedia.org/wiki/DIN_5008
  normalFont = getFont("print_font_normal");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(borderLeft(),              // left margin
                             getPoints(120),            // top margin
                             (width() - borderRight()), // right margin
                             getPoints(100));

  QString _css("* {background-color:#FFFFFF;color:#000000;border:none;}");

  m_table = new QTableWidget(this);
  m_table->setColumnCount(3);
  m_table->setStyleSheet(_css);
  m_table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  m_table->setSelectionMode(QAbstractItemView::NoSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->setWordWrap(true);

  setTableHeader(0, tr("Article"));
  setTableHeader(1, tr("Description"));
  setTableHeader(2, tr("Mengle"));

  QHeaderView *m_hview = m_table->horizontalHeader();
  m_hview->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_hview->setSectionResizeMode(1, QHeaderView::Stretch);

  QHeaderView *m_vview = m_table->verticalHeader();
  m_vview->setVisible(false);
  m_vview->setSectionResizeMode(QHeaderView::ResizeToContents);

  layout->addWidget(m_table);
  setLayout(layout);
}

void DeliveryNote::setTableHeader(int column, const QString &title) {
  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setForeground(Qt::black);
  item->setBackground(Qt::white);
  switch (column) {
  case 0:
    item->setTextAlignment(Qt::AlignCenter);
    break;

  case 2:
    item->setTextAlignment(Qt::AlignCenter);
    break;

  default:
    item->setTextAlignment(Qt::AlignLeft);
    break;
  }
  m_table->setHorizontalHeaderItem(column, item);
}

const QPoint DeliveryNote::startPoint() const {
  return QPoint(borderLeft(), getPoints(100));
}

void DeliveryNote::setData(int row, int column, const QString &title) {
  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setForeground(Qt::black);
  item->setBackground(Qt::white);
  if (column == 1) {
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
  } else {
    item->setTextAlignment(Qt::AlignCenter);
  }
  m_table->setItem(row, column, item);
}

void DeliveryNote::paintSubject(QPainter &painter) {
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

void DeliveryNote::paintContent(QPainter &painter) {
  if (!contentData.contains("body"))
    return; // skip if not exists

  // at first paint subject and date with underline.
  paintSubject(painter);

  // dataset
  const QJsonObject _body = contentData.value("body").toObject();

  // config
  // qreal _fontHeight = fontHeight(normalFont);
  qreal _y = position;
  qreal _x = borderLeft();
  // qreal _right_x = borderRight();
  QTransform _transform = painter.transform();

  // init painter
  painter.setPen(fontPen());
  painter.setFont(normalFont);

  if (_body.contains("intro")) {
    QStaticText _intro = textBlock();
    _intro.setText(_body.value("intro").toString());
    _intro.prepare(_transform, normalFont);
    painter.drawStaticText(QPoint(_x, _y), _intro);
  }
}

bool DeliveryNote::setContentData(QJsonObject &data) {
  bool _return = false;
  if (!data.contains("config")) {
    qWarning("Unable to read invoice delivery note data!");
    return _return;
  }

  if (!data.contains("config"))
    return false;

  contentData = data;
  QJsonObject _config = contentData.value("config").toObject();
  QJsonObject _body;
  _body.insert("intro", companyData("COMPANY_INTRO_DELIVERY"));
  _body.insert("policy", companyData("COMPANY_RETURN_POLICY"));
  contentData.insert("body", _body);

  AntiquaCRM::ASqlFiles _tpl("query_printing_delivery_note");
  if (!_tpl.openTemplate()) {
    qWarning("Unable to open delivery note SQL template!");
  }

  QString _sql("a_order_id=");
  _sql.append(QString::number(_config.value("order_id").toDouble()));
  _sql.append(" AND a_customer_id=");
  _sql.append(QString::number(_config.value("customer_id").toDouble()));
  _tpl.setWhereClause(_sql);

  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  if (_query.size() > 0) {
    m_table->setRowCount(_query.size());
    int _row = 0;
    while (_query.next()) {
      setData(_row, 0, _query.value("aid").toString());
      setData(_row, 1, _query.value("title").toString());
      setData(_row, 2, _query.value("crowd").toString());
      _row++;
    }
    _query.clear();
    return true;
  }
  return false;
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
