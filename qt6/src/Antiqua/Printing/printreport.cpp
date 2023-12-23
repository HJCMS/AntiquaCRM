// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printreport.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QPagedPaintDevice>
#include <QPdfWriter>
#include <QPrintDialog>

namespace AntiquaCRM {

PrintReportPage::PrintReportPage(QWidget *parent, QPageLayout pl)
    : QWidget{parent}, APrintTools{pl.pageSize().id()}, p_pageLayout{pl} {
  setContentsMargins(0, 0, 0, 0);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(contentsMargins());
  body = new APrintingBody(this);
  body->setContentsMargins(contentsMargins());
  body->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(body);
  setLayout(layout);
}

bool PrintReportPage::setContentData(QJsonObject &data) {
  QTextCursor cursor = body->textCursor();
  body->insertText(cursor, data.value("info").toString());

  QTextTableFormat _tFormat = body->tableFormat();
  _tFormat.setBottomMargin(10);

  QJsonObject _header = data.value("header").toObject();
  QJsonArray _data = data.value("sold").toArray();
  int _columns = _header.size();
  int _rows = _data.size();
  QTextTable *table = cursor.insertTable(1, _columns, _tFormat);
  for (int c = 0; c < _columns; c++) {
    QTextTableCell _tc = table->cellAt(0, c);
    _tc.setFormat(body->tableCellFormat());
    QString _txt = _header.value(QString::number(c)).toString();
    body->setCellItem(_tc, _txt, Qt::AlignCenter);
  }
  table->appendRows(_rows);
  for (int r = 1; r < (_rows + 1); r++) {
    QJsonObject _line = _data[r].toObject();
    for (int c = 0; c < _columns; c++) {
      QTextTableCell _tc = table->cellAt(r, c);
      _tc.setFormat(body->tableCellFormat());
      QString _txt = _line.value(QString::number(c)).toString();
      body->setCellItem(_tc, _txt, Qt::AlignCenter);
    }
  }

  QTextTableCell _stc = table->cellAt(_rows, (_columns - 2));
  _stc.setFormat(body->tableCellFormat());
  body->setCellItem(_stc, tr("Total") + ":", Qt::AlignRight);

  QTextTableCell _ttc = table->cellAt(_rows, (_columns - 1));
  _ttc.setFormat(body->tableCellFormat());

  QString _txt = data.value("money").toString();
  body->setCellItem(_ttc, _txt, Qt::AlignCenter);
  body->document()->setModified(true);
  update();
  return true;
}

PrintReport::PrintReport(QWidget *parent) : AntiquaCRM::APrintDialog{parent} {
  setObjectName("print_report_dialog");

  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize::A4, QMarginsF(30, 20, 20, 20));
  pageLayout.setMode(QPageLayout::StandardMode);
  pageLayout.setUnits(QPageLayout::Millimeter);

  m_page = new PrintReportPage(this, pageLayout);
  viewPort->setWidget(m_page);
}

void PrintReport::renderPage(QPrinter *printer) {
  printer->setPageLayout(pageLayout);
  m_page->body->print(printer);
}

void PrintReport::createPDF() {
  const QDir _dir = config->getArchivPath(ANTIQUACRM_ARCHIVE_REPORTS);
  if (!_dir.exists()) {
    qWarning("Bookcard destination not exists or writeable!");
    return;
  }

  QFileInfo _file(_dir, pdfFileName);
  QPrinter *printer = new QPrinter(QPrinter::HighResolution);
  printer->setPageLayout(pageLayout);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setCreator("AntiquaCRM");
  printer->setOutputFileName(_file.filePath());
  m_page->body->print(printer);

  if (_file.isReadable())
    sendStatusMessage(tr("PDF Document created!"));
}

void PrintReport::openPrintDialog() {
  QPrinterInfo p_info;
  QString _device = config->value("printer/device_secondary").toString();
  if (_device.isEmpty()) {
    p_info = QPrinterInfo::defaultPrinter();
  } else {
    p_info = QPrinterInfo::printerInfo(_device);
  }

  QPrinter *printer = new QPrinter(p_info, QPrinter::ScreenResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setDocName(pdfFileName);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  dialog->setOption(QAbstractPrintDialog::PrintShowPageSize, true);
  connect(dialog, SIGNAL(accepted(QPrinter *)),
          SLOT(renderPage(QPrinter *)));

  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

int PrintReport::exec(const QJsonObject &opts, bool pdfbtn) {
  btn_pdf->setEnabled(pdfbtn);

  QJsonObject _obj = opts;
  pdfFileName = _obj.value("file").toString();
  pdfFileName.append(".pdf");
  m_page->setContentData(_obj);
  return QDialog::exec();
}

} // namespace AntiquaCRM
