// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QLayout>
#include <QPrintDialog>
#include <QTableWidgetItem>

namespace AntiquaCRM {

DeliveryNote::DeliveryNote(QWidget *parent)
    : AntiquaCRM::APrintingPage{parent} {
  setObjectName("printing_deliverynote_page");
  normalFont = getFont("print_font_normal");

  QVBoxLayout *layout = new QVBoxLayout(this);
  // https://de.wikipedia.org/wiki/DIN_5008
  qreal _top = getPoints(100) + fontHeight(normalFont) + linePen().width();
  layout->setContentsMargins(borderLeft(),              // left margin
                             _top,                      // top margin
                             (width() - borderRight()), // right margin
                             getPoints(50));

  QString _css("* {background-color:#FFFFFF;color:#000000;border:none;}");

  m_intro = new QLabel(this);
  m_intro->setContentsMargins(5, 5, 5, 5);
  m_intro->setWordWrap(true);
  m_intro->setStyleSheet(_css);
  layout->insertWidget(0, m_intro);

  m_table = new QTableWidget(this);
  m_table->setColumnCount(3);
  m_table->setStyleSheet(_css);
  m_table->setSelectionMode(QAbstractItemView::NoSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  setArticleHeaderItem(0, tr("Article"), (Qt::AlignCenter));
  setArticleHeaderItem(1, tr("Description"), (Qt::AlignLeft));
  setArticleHeaderItem(2, tr("Amount"), (Qt::AlignCenter));

  QHeaderView *m_hview = m_table->horizontalHeader();
  m_hview->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_hview->setSectionResizeMode(1, QHeaderView::Stretch);

  QHeaderView *m_vview = m_table->verticalHeader();
  m_vview->setVisible(false);
  m_vview->setSectionResizeMode(QHeaderView::ResizeToContents);

  layout->insertWidget(1, m_table);
  layout->setStretch(1, 1);

  m_final = new QLabel(this);
  m_final->setContentsMargins(5, 5, 5, 5);
  m_final->setWordWrap(true);
  m_final->setStyleSheet(_css);
  layout->insertWidget(2, m_final);

  layout->addStretch(1);
  setLayout(layout);
}

const QPoint DeliveryNote::startPoint() const {
  return QPoint(borderLeft(), getPoints(100));
}

void DeliveryNote::setArticleData(int row, int column, const QVariant &data) {
  // Table Rows
  QTableWidgetItem *item = new QTableWidgetItem;
  switch (column) {
  case 0:
    item->setText(data.toString());
    item->setTextAlignment(Qt::AlignRight);
    break;

  case 1:
    item->setText(data.toString());
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    break;

  default:
    item->setText(data.toString());
    item->setTextAlignment(Qt::AlignCenter);
    break;
  }
  m_table->setItem(row, column, item);
}

void DeliveryNote::setSummary() {
  // Summary
  int _count = m_table->rowCount();
  int _summary = 0;
  for (int r = 0; r < _count; r++) {
    int _c = m_table->item(r, 2)->data(Qt::DisplayRole).toInt();
    _summary += _c;
  }
  m_table->setRowCount(_count + 1);

  QTableWidgetItem *ic1 = new QTableWidgetItem(tr("Summary"));
  ic1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_table->setItem(_count, 1, ic1);

  QTableWidgetItem *ic2 = new QTableWidgetItem(QString::number(_summary));
  ic2->setTextAlignment(Qt::AlignCenter);
  m_table->setItem(_count, 2, ic2);
}

void DeliveryNote::paintContent(QPainter &painter) {
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

bool DeliveryNote::setContentData(QJsonObject &data) {
  if (!data.contains("config")) {
    qWarning("Unable to read invoice delivery note data!");
    return false;
  }
  contentData = data;
  m_intro->setText(companyData("COMPANY_DELIVERY_INTRO"));
  m_final->setText(companyData("COMPANY_DELIVERY_THANKS"));

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
  if (_query.size() > 0) {
    m_table->setRowCount(_query.size());
    int _row = 0;
    while (_query.next()) {
      setArticleData(_row, 0, _query.value("aid").toString());
      setArticleData(_row, 1, _query.value("title").toString());
      setArticleData(_row, 2, _query.value("crowd").toString());
      _row++;
    }
    setSummary();
    _query.clear();
  }
  return (m_table->rowCount() > 0);
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
