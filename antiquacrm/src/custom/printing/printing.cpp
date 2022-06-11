// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printing.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "texteditor.h"

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QPrinterInfo>
#include <QProcess>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtGlobal>

Printing::Printing(QWidget *parent) : QDialog{parent} {
  setObjectName("printing_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(640, 580);

  config = new ApplSettings(this);

  headerFont = QFont("URW Chancery L [urw]", 26);
  normalFont = QFont("Tahoma", 11);
  footerFont = QFont("Tahoma", 10);
  smallFont = QFont("Tahoma", 8);
  // 210 x 297 mm, 8.26 x 11.69 inches
  page_margins = QMarginsF(10, 1, 1, 1);
  page_size = QPageSize(QPageSize::A4Plus);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_layout");

  QRect pageRect = pageLayout().fullRectPoints();
  int maxWidth = pageRect.width();

  QScrollArea *scrollAera = new QScrollArea(this);
  scrollAera->setObjectName("scroll_area");
  layout->addWidget(scrollAera);

  printArea = new QWidget(scrollAera);
  printArea->setObjectName("printing_area");
  printArea->setContentsMargins(0, 0, 0, 0);
  printArea->setStyleSheet("border:none;");
  printArea->setFixedSize(pageRect.size());
  QVBoxLayout *frame_layout = new QVBoxLayout(printArea);
  frame_layout->setContentsMargins(0, 0, 0, 0);
  frame_layout->setSizeConstraint(QLayout::SetMinimumSize);
  frame_layout->setSpacing(0);

  header = new TextEditor(printArea);
  header->setObjectName("printing_header");
  header->setMinimumHeight(25);
  header->setMaximumWidth(maxWidth);
  header->setTextColor(Qt::black);
  header->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(header);
  body = new TextEditor(printArea);
  body->setObjectName("printing_body");
  body->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
  body->setMaximumWidth(maxWidth);
  body->setTextColor(Qt::black);
  frame_layout->addWidget(body);
  footer = new TextEditor(printArea);
  footer->setObjectName("printing_footer");
  footer->setMinimumHeight(20);
  footer->setMaximumWidth(maxWidth);
  footer->setTextColor(Qt::black);
  footer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(footer);
  frame_layout->setStretchFactor(header, 30);
  frame_layout->setStretchFactor(body, 40);
  frame_layout->setStretchFactor(footer, 20);
  scrollAera->setWidget(printArea);

  QWidget *buttonWidget = new QWidget(this);
  QHBoxLayout *hLayout = new QHBoxLayout(buttonWidget);
  selectPrinter = new QComboBox(buttonWidget);
  hLayout->addWidget(selectPrinter);
  hLayout->addStretch(1);
  printButton = new QPushButton(tr("Printing"), buttonWidget);
  printButton->setIcon(myIcon("printer"));
  hLayout->addWidget(printButton);
  quitButton = new QPushButton(tr("Close"), buttonWidget);
  quitButton->setIcon(myIcon("exit"));
  hLayout->addWidget(quitButton);
  layout->addWidget(buttonWidget);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(quitButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(selectPrinter, SIGNAL(currentIndexChanged(int)), this,
          SLOT(printerSelected(int)));
  connect(this, SIGNAL(statusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

void Printing::readConfiguration() {
  config->beginGroup("company");
  QStringList keys = config->childKeys();
  if (keys.count() < 1) {
    QString warn("<p>");
    warn.append(tr("Your Company configuration is incomplete!"));
    warn.append("</p>");
    warn.append(tr("Edit Company settings first!"));
    warningMessageBox(warn);
    return;
  }
  foreach (QString k, keys) {
    companyData.insert(k, config->value(k).toString());
  }
  config->endGroup();
  config->beginGroup("printer");
  QFont font;
  if (font.fromString(config->value("header_font").toString())) {
    headerFont.swap(font);
  }
  if (font.fromString(config->value("normal_font").toString())) {
    normalFont.swap(font);
  }
  if (font.fromString(config->value("footer_font").toString())) {
    footerFont.swap(font);
  }
  if (font.fromString(config->value("small_font").toString())) {
    smallFont.swap(font);
  }
  config->endGroup();
}

const QTextCharFormat Printing::headerFormat() {
  QTextCharFormat f;
  f.setFont(headerFont);
  return f;
}

const QTextCharFormat Printing::normalFormat() {
  QTextCharFormat f;
  f.setFont(normalFont);
  return f;
}

const QTextCharFormat Printing::footerFormat() {
  QTextCharFormat f;
  f.setFont(footerFont);
  return f;
}

const QTextCharFormat Printing::smallFormat() {
  QTextCharFormat f;
  f.setFont(smallFont);
  return f;
}

const QTextTableFormat Printing::tableFormat() {
  QTextTableFormat f;
  f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  f.setCellPadding(5);
  f.setCellSpacing(0);
  f.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  f.setTopMargin(5);
  return f;
}

void Printing::constructHeader() {
  QTextBlockFormat block;
  block.setAlignment(Qt::AlignCenter);
  QTextCursor cursor = header->textCursor();
  cursor.setCharFormat(headerFormat());
  cursor.insertBlock(block);
  QString str = companyData.value("name");
  cursor.insertText(str.replace("#", "\n"));
  cursor.atEnd();
  int w = header->size().width();
  int fh = cursor.blockCharFormat().font().pointSize();
  int lc = header->document()->lineCount();
  int h = (fh * (lc + 1));
  header->resize(QSize(w, h));
  header->update();
  header->setMaximumHeight(h);
  header->setReadOnly(true);
  header->document()->setModified(true);
}

void Printing::constructFooter() {
  QTextCursor cursor = footer->textCursor();
  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setBorderBrush(Qt::black);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  // FOOTER
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(0);
  QTextTable *table = cursor.insertTable(1, 2, format);
  table->setObjectName("footer_table");

  QTextTableCell ce00 = table->cellAt(0, 0);
  ce00.setFormat(cellFormat);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(footerFormat());
  QString name = companyData.value("name");
  cursor.insertText(name.replace("#", " ") + "\n");
  cursor.insertText(companyData.value("street") + " ");
  cursor.insertText(companyData.value("location") + "\n");
  cursor.insertText(tr("email") + ": ");
  cursor.insertText(companyData.value("email") + "\n");
  cursor.insertText(tr("phone") + ": ");
  cursor.insertText(companyData.value("phone") + "\n");
  cursor.insertText(tr("fax") + ": ");
  cursor.insertText(companyData.value("fax") + "\n");

  QTextTableCell ce01 = table->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(footerFormat());
  cursor.insertText(companyData.value("bank") + "\n");
  cursor.insertText("SWIFT-BIC: ");
  cursor.insertText(companyData.value("bicswift") + "\n");
  cursor.insertText("IBAN: ");
  cursor.insertText(companyData.value("iban") + "\n");
  cursor.insertText(tr("tax number") + ": ");
  cursor.insertText(companyData.value("taxnumber") + "\n");

  cursor = footer->textCursor();
  cursor.atEnd();

  int w = footer->size().width();
  int h = (footer->document()->lineCount() * footer->cursorRect().height());
  footer->resize(QSize(w, h));
  footer->update();
  footer->setMaximumHeight(h);
  footer->setReadOnly(true);
  footer->document()->setModified(true);
}

const QString Printing::outputDirectory(const QString &target) {
  QVariant dest = config->value("dirs/" + target, QDir::homePath());
  return dest.toString();
}

const QString Printing::getHeaderHTML() {
  return header->document()->toHtml(defaultCodec());
}

const QString Printing::getBodyHTML() {
  return body->document()->toHtml(defaultCodec());
}

const QString Printing::getFooterHTML() {
  return footer->document()->toHtml(defaultCodec());
}

const QImage Printing::getWatermark() {
  QStringList accept({"PNG", "JPG"});
  QString file(config->value("printer/attachments").toString());
  file.append(QDir::separator());
  file.append(config->value("printer/watermark").toString());
  QFileInfo info(file);
  if (info.isFile() && info.isReadable()) {
    QString type = info.completeSuffix().split(".").last().toUpper();
    if (!accept.contains(type, Qt::CaseSensitive)) {
      qWarning("unsupported image type");
      return QImage();
    }
    QByteArray buffer;
    QFile fp(info.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      while (!fp.atEnd()) {
        buffer += fp.readLine();
      }
    }
    fp.close();
    return QImage::fromData(buffer, type.toLocal8Bit());
  }
  return QImage();
}

void Printing::addPrinters() {
  int index = 0;
  selectPrinter->clear();
  selectPrinter->insertItem(index++, tr("Printers"));
  QListIterator<QPrinterInfo> it(QPrinterInfo::availablePrinters());
  while (it.hasNext()) {
    QPrinterInfo info = it.next();
    selectPrinter->insertItem(index, myIcon("printer"), info.printerName(),
                              info.isDefault());
    if (info.isDefault())
      selectPrinter->setCurrentIndex(index);

    index++;
  }
}

void Printing::printerSelected(int index) {
  QString name = selectPrinter->itemData(index, Qt::DisplayRole).toString();
  QPrinterInfo info = QPrinterInfo::printerInfo(name);
  if (!info.isNull()) {
    p_printerName = name;
    emit printerChanged(true);
    emit statusMessage(info.description());
    return;
  }
  p_printerName = QString();
}

const QPageSize Printing::pageSize() { return page_size; }

const QPageLayout Printing::pageLayout() {
  QPageLayout pageLayout;
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(page_size, page_margins);
  pageLayout.setMode(QPageLayout::FullPageMode);
  pageLayout.setUnits(QPageLayout::Millimeter);
  return pageLayout;
}

bool Printing::fontFamilyExists(const QString &family) {
  foreach (QString f, QFontDatabase().families(QFontDatabase::Latin)) {
    if (f.contains(family, Qt::CaseSensitive)) {
      return true;
    }
  }
  return false;
}

void Printing::setHeaderFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    headerFont = font;
    emit headerFontChanged();
  }
}

const QFont Printing::getHeaderFont() { return headerFont; }

void Printing::setNormalFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    normalFont = font;
    emit normalFontChanged();
  }
}

const QFont Printing::getNormalFont() { return normalFont; }

void Printing::setFooterFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    footerFont = font;
    emit footerFontChanged();
  }
}

const QFont Printing::getFooterFont() { return footerFont; }

void Printing::setSmallFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    smallFont = font;
    emit smallFontChanged();
  }
}

const QFont Printing::getSmallFont() { return smallFont; }

int Printing::warningMessageBox(const QString &txt) {
  return QMessageBox::warning(this, tr("Delivery note"), txt, QMessageBox::Ok);
}

void Printing::setCustomerAddress(const QString &address) {
  if (address.isEmpty()) {
    qWarning("empty customer address");
    return;
  }
  p_customerAddress = address;
}

const QByteArray Printing::defaultCodec() {
  QTextCodec *c = QTextCodec::codecForLocale();
  return c->name();
}
