// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reports.h"
#include "texteditor.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QIcon>
#include <QLayout>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPushButton>
#include <QTextCursor>

Reports::Reports(QWidget *parent) : QDialog{parent} {
  setObjectName("printing_reports");
  setWindowTitle(tr("Reports"));
  setSizeGripEnabled(true);
  setContentsMargins(5, 5, 5, 10);
  setMinimumHeight(550);

  p_pageLayout.setOrientation(QPageLayout::Portrait);
  p_pageLayout.setPageSize(QPageSize(QPageSize::A4), QMarginsF(0, 0, 0, 0));
  p_pageLayout.setMode(QPageLayout::StandardMode);
  p_pageLayout.setUnits(QPageLayout::Millimeter);
  p_pageLayout.setTopMargin(0);

  QRectF pageRect = p_pageLayout.pageSize().rect(QPageSize::Point);
  int pageWidth = pageRect.width();
  setMinimumWidth(pageWidth + 10);
  setMaximumWidth(pageWidth + 20);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_card_layout");
  layout->setContentsMargins(0, 0, 0, 0);

  m_edit = new TextEditor(this);
  m_edit->setContentsMargins(0, 0, 0, 0);
  m_edit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  m_edit->setMinimumWidth(pageWidth);
  m_edit->setTextColor(Qt::black);
  layout->addWidget(m_edit);

  QDialogButtonBox *btn_box = new QDialogButtonBox(this);
  btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  QPushButton *btn_save = btn_box->button(QDialogButtonBox::Ok);
  btn_save->setIcon(QIcon("://icons/action_ok.png"));

  QPushButton *btn_close = btn_box->button(QDialogButtonBox::Close);
  btn_close->setIcon(QIcon("://icons/action_quit.png"));

  layout->addWidget(btn_box);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(btn_box, SIGNAL(accepted()), this, SLOT(openPrintDialog()));
  connect(btn_box, SIGNAL(rejected()), this, SLOT(reject()));
  connect(this, SIGNAL(statusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

void Reports::printerConfiguration() {
  config = new AntiquaCRM::ASettings(this);
  config->beginGroup("printer");
  headerFont = m_edit->font();
  headerFont.fromString(config->value("normal_font").toString());
  defaultFont = m_edit->font();
  defaultFont.fromString(config->value("small_font").toString());
  p_printerName = config->value("DIN_A4_Printer").toString();
  config->endGroup();

  if (p_printerName.isEmpty()) {
    qWarning("No Printer configuration start search default.");
    QListIterator<QPrinterInfo> printers(QPrinterInfo::availablePrinters());
    while (printers.hasNext()) {
      QPrinterInfo pr = printers.next();
      if (pr.isDefault()) {
        p_printerName = pr.printerName();
        break;
      }
    }
  }

  QString dstr = QDateTime::currentDateTime().toString("yyyy.MM.dd_HHmm");
  QString dest = config->value("dirs/reports").toString();
  dest.append(QDir::separator());
  dest.append(tr("monthlyreport") + "_" + dstr);
  p_filePath = dest.trimmed();
}

const QTextTableFormat Reports::tableFormat() {
  QTextTableFormat f;
  f.setBorder(0);
  f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  f.setCellPadding(0);
  f.setCellSpacing(0);
  f.setTopMargin(0);
  f.setLeftMargin(0);
  f.setRightMargin(0);
  f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  f.setAlignment(Qt::AlignCenter);
  return f;
}

const QTextTableCellFormat Reports::cellFormat() {
  QTextTableCellFormat f;
  f.setBorder(0);
  f.setBottomBorder(1);
  QBrush brush(Qt::SolidPattern);
  brush.setColor(Qt::gray);
  f.setBottomBorderBrush(brush);
  f.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  return f;
}

const QTextCharFormat Reports::headerFormat() {
  QFont font(headerFont);
  font.setBold(true);

  QTextCharFormat format;
  format.setFont(font);
  return format;
}

const QTextCharFormat Reports::textFormat(qint8 size) {
  QFont font(defaultFont);
  if (size != 0)
    font.setPointSize(size);

  QTextCharFormat format;
  format.setFont(font);
  format.setVerticalAlignment(QTextCharFormat::AlignMiddle);
  return format;
}

void Reports::constructSubject() {
  QTextCursor cursor = m_edit->textCursor();

  QTextBlockFormat block;
  block.setTopMargin(0);
  block.setIndent(0);
  block.setAlignment(Qt::AlignCenter);
  cursor.setCharFormat(headerFormat());

  cursor.insertBlock(block);
  cursor.insertText(p_title);
  cursor.atEnd();

  QTextTableFormat format = tableFormat();
  int rows = (p_data.size() + 1);
  m_table = cursor.insertTable(rows, p_header.count(), format);
  m_table->setObjectName("report_table");

  int cell = 0;
  foreach (QString t, p_header) {
    QTextTableCell c = m_table->cellAt(0, cell++);
    c.setFormat(cellFormat());
    cursor = c.firstCursorPosition();
    cursor.setCharFormat(textFormat());
    cursor.insertText(t);
  }
}

void Reports::constructBody() {
  constructSubject();
  QTextCursor cursor = m_edit->textCursor();
  int r = 1; // 0 = Headerzeile
  foreach (QString row, p_data) {
    int c = 0;
    foreach (QString item, row.split(p_delimiter)) {
      QTextTableCell cell = m_table->cellAt(r, c);
      cell.setFormat(cellFormat());
      cursor = cell.firstCursorPosition();
      cursor.setCharFormat(textFormat());
      cursor.insertText(item);
      c++;
    }
    r++;
  }
  cursor = m_edit->textCursor();
  cursor.insertText("\n");
  m_edit->document()->setModified(true);
}

bool Reports::generateDocument(QPrinter *printer) {
  printer->setPageLayout(p_pageLayout);
  m_edit->print(printer);
  return true;
}

bool Reports::createPDF() {
  QPrinter *printer = new QPrinter(QPrinter::HighResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setFullPage(true);
  printer->setCreator("AntiquaCRM");

  QFileInfo outFile(p_filePath + ".pdf");
  printer->setDocName(outFile.fileName());
  printer->setOutputFileName(outFile.filePath());

  return generateDocument(printer);
}

void Reports::openPrintDialog() {
  if (p_printerName.isEmpty()) {
    if (createPDF()) {
      emit statusMessage(tr("PDF File written."));
    } else {
      qWarning("PDF not generated");
    }
    return;
  }

  QPrinterInfo p_info = QPrinterInfo::printerInfo(p_printerName);
  QPrinter *printer = new QPrinter(p_info, QPrinter::PrinterResolution);
  printer->setPageLayout(p_pageLayout);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPrinterName(p_printerName);
  printer->setCreator("AntiquaCRM");
  printer->setFullPage(true);

  QFileInfo outFile(p_filePath + ".pdf");
  printer->setDocName(outFile.fileName());

  QPrintDialog *dialog = new QPrintDialog(printer, m_edit);
  connect(dialog, SIGNAL(accepted(QPrinter *)),
          SLOT(generateDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

void Reports::setHeaderInfo(const QString &title, const QString &header) {
  p_header = header.split(p_delimiter);
  p_title = title;
  m_edit->setDocumentTitle(title);
}

int Reports::exec() {
  qWarning("you must call exec with params!");
  return QDialog::Rejected;
}

int Reports::exec(const QStringList &rows) {
  if (rows.size() < 1) {
    qWarning("Nothing todo!");
    return QDialog::Rejected;
  }

  p_data = rows;

  printerConfiguration();

  constructBody();

  return QDialog::exec();
}
