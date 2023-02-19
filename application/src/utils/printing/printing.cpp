// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printing.h"
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

  config = new AntiquaCRM::ASettings(this);

  headerFont = QFont("URW Chancery L [urw]", 26);
  normalFont = QFont("Tahoma", 11);
  footerFont = QFont("Tahoma", 10);
  smallFont = QFont("Tahoma", 8);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_layout");

  QRectF pageRect = QPageSize(QPageSize::A4).rect(QPageSize::Point);
  int pageWidth = pageRect.width();

  QScrollArea *scrollAera = new QScrollArea(this);
  scrollAera->setObjectName("scroll_area");
  layout->addWidget(scrollAera);

  printArea = new QWidget(scrollAera);
  printArea->setObjectName("printing_area");
  printArea->setContentsMargins(0, 0, 0, 0);
  printArea->setStyleSheet("border:none;");
  printArea->setFixedSize(pageRect.toRect().size());
  QVBoxLayout *frame_layout = new QVBoxLayout(printArea);
  frame_layout->setContentsMargins(0, 0, 0, 0);
  frame_layout->setSizeConstraint(QLayout::SetMinimumSize);
  frame_layout->setSpacing(0);

  header = new TextEditor(printArea);
  header->setObjectName("printing_header");
  header->setContentsMargins(0, 0, 0, 0);
  header->setMinimumHeight(25);
  header->setMinimumWidth(pageWidth);
  header->setMaximumWidth(pageWidth);
  header->setTextColor(Qt::black);
  header->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(header);
  body = new TextEditor(printArea);
  body->setObjectName("printing_body");
  body->setContentsMargins(0, 0, 0, 0);
  body->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
  body->setMinimumWidth(pageWidth);
  body->setMaximumWidth(pageWidth);
  body->setTextColor(Qt::black);
  frame_layout->addWidget(body);
  footer = new TextEditor(printArea);
  footer->setObjectName("printing_footer");
  footer->setContentsMargins(0, 0, 0, 0);
  footer->setMinimumHeight(20);
  footer->setMinimumWidth(pageWidth);
  footer->setMaximumWidth(pageWidth);
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
  pdfButton = new QPushButton(tr("Create PDF"), buttonWidget);
  pdfButton->setIcon(QIcon("://icons/printer.png"));
  pdfButton->setEnabled(false);
  hLayout->addWidget(pdfButton);
  printButton = new QPushButton(tr("Printing"), buttonWidget);
  printButton->setIcon(QIcon("://icons/printer.png"));
  hLayout->addWidget(printButton);
  quitButton = new QPushButton(tr("Close"), buttonWidget);
  quitButton->setIcon(QIcon("://icons/action_quit.png"));
  hLayout->addWidget(quitButton);
  layout->addWidget(buttonWidget);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(quitButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(selectPrinter, SIGNAL(currentIndexChanged(int)),
          SLOT(printerSelected(int)));
  connect(this, SIGNAL(statusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

void Printing::readConfiguration() {
  config->beginGroup("payment");
  p_currency = config->value("currency").toByteArray();
  p_tax_value = config->value("vat1").toInt();
  p_grace_period = config->value("grace_period").toString();
  config->endGroup();

  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery q = sql.query("SELECT ac_class,ac_value FROM antiquacrm_company;");
  if (q.size() > 0) {
    while (q.next()) {
      QString key = q.value("ac_class").toString().toUpper();
      QString value = q.value("ac_value").toString();
      // qDebug() << key << ":" << value;
      companyData.insert(key, value);
    }
  }

  config->beginGroup("printer");
  QFont font;
  if (font.fromString(config->value("header_font").toString())) {
    headerFont.swap(font);
  }
  if (font.fromString(config->value("address_font").toString())) {
    addressFont.swap(font);
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

const QString Printing::displayPrice(double price) {
  QString str = QString::number(price, 'f', 2);
  str.append(" " + QString(p_currency));
  return str;
}

const QTextCharFormat Printing::headerFormat() {
  QTextCharFormat f;
  f.setFont(headerFont);
  return f;
}

const QTextCharFormat Printing::addressFormat() {
  QTextCharFormat f;
  f.setFont(addressFont);
  return f;
}

const QTextCharFormat Printing::normalFormat() {
  QTextCharFormat f;
  f.setFont(normalFont);
  return f;
}

const QTextCharFormat Printing::boldFormat() {
  QTextCharFormat f;
  QFont font(normalFont);
  font.setBold(true);
  f.setFont(font);
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

const QTextBlockFormat Printing::alignRight() {
  QTextBlockFormat tbf;
  tbf.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return tbf;
}

const QTextBlockFormat Printing::alignCenter() {
  QTextBlockFormat tbf;
  tbf.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return tbf;
}

const QTextTableFormat Printing::tableFormat() {
  QTextTableFormat f;
  f.setWidth(QTextLength(QTextLength().PercentageLength, 95));
  f.setPadding(0);
  f.setCellPadding(0);
  f.setCellSpacing(0);
  f.setTopMargin(0);
  f.setBottomMargin(0);
  f.setRightMargin(15.0); // 15 Pixel
  f.setBorder(0.0);
  f.setBorderBrush(QBrush(Qt::NoBrush));
  f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  f.setAlignment(Qt::AlignRight | Qt::AlignTop);
  return f;
}

const QTextTableCellFormat Printing::cellFormat(Printing::Border border) {
  QTextTableCellFormat f;
  f.setBorderBrush(borderBrush());
  f.setPadding(0);
  if (border == Printing::Border::Top) {
    f.setTopBorder(1);
    f.setBottomBorder(0);
    f.setBorderBrush(borderBrush());
    f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  } else if (border == Printing::Border::Bottom) {
    f.setTopBorder(0);
    f.setBottomBorder(1);
    f.setBorderBrush(borderBrush());
    f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  } else {
    f.setTopBorder(0);
    f.setBottomBorder(0);
    f.setBorderBrush(QBrush(Qt::NoBrush));
    f.setTopBorderStyle(QTextFrameFormat::BorderStyle_None);
  }
  return f;
}

const QBrush Printing::borderBrush() {
  return QBrush(Qt::darkGray, Qt::SolidPattern);
}

QTextFrame *Printing::bodyFrame() {
  QTextFrame *frame = new QTextFrame(body->document());
  return frame;
}

void Printing::constructHeader() {
  QTextBlockFormat block;
  block.setAlignment(Qt::AlignCenter);
  QTextCursor cursor = header->textCursor();
  cursor.setCharFormat(headerFormat());
  QString title = companyData.value("COMPANY_PRINTING_HEADER");
  foreach (QString line, title.split("\n")) {
    cursor.insertBlock(block);
    cursor.insertText(line);
    cursor.atEnd();
  }
  QFontMetricsF fm(headerFont);
  int w = header->size().width();
  int h = qRound(fm.height() * (header->document()->lineCount() + 1));
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
  QTextTable *table = cursor.insertTable(1, 2, format);
  table->setObjectName("footer_table");

  QTextTableCell ce00 = table->cellAt(0, 0);
  ce00.setFormat(cellFormat);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(footerFormat());
  QString name = companyData.value("COMPANY_FULLNAME");
  cursor.insertText(name.replace("#", " ") + "\n");
  cursor.insertText(companyData.value("COMPANY_STREET") + " ");
  cursor.insertText(companyData.value("COMPANY_LOCATION") + "\n");
  cursor.insertText(tr("eMail") + ": ");
  cursor.insertText(companyData.value("COMPANY_EMAIL") + "\n");
  cursor.insertText(tr("Phone") + ": ");
  cursor.insertText(companyData.value("COMPANY_PHONE") + "\n");
  cursor.insertText(tr("Fax") + ": ");
  cursor.insertText(companyData.value("COMPANY_FAX") + "\n");

  QTextTableCell ce01 = table->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(footerFormat());
  cursor.insertText(companyData.value("COMPANY_BANK_NAME") + "\n");
  cursor.insertText("Swift-BIC: ");
  cursor.insertText(companyData.value("COMPANY_BANK_BICSWIFT") + "\n");
  cursor.insertText("IBAN: ");
  cursor.insertText(companyData.value("COMPANY_BANK_IBAN") + "\n");
  cursor.insertText(tr("Tax Number") + ": ");
  cursor.insertText(companyData.value("COMPANY_TAX_NUMBER") + "\n");
  cursor.insertText(tr("VAT Number") + ": ");
  cursor.insertText(companyData.value("COMPANY_VAT_NUMBER") + "\n");
  if (!companyData.value("COMPANY_LEGALITY").isEmpty()) {
    cursor.insertText(tr("Legality") + ": ");
    cursor.insertText(companyData.value("COMPANY_LEGALITY") + "\n");
  }
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

bool Printing::addArticleRow(QTextTable *table, BillingInfo article) {
  int row = table->rows();
  table->insertRows(row, 3);
  p_articleCalc.insert(article.articleid, article.vatValue);
  if (article.vatValue != p_tax_value)
    p_tax_value = article.vatValue;

  // Begin:PrepareData
  QString _currency(" " + p_currency);
  // Preisausgabe
  double _total = 0.00;
  double _price = article.sellPrice;
  if (article.quantity > 1) {
    _price = (article.quantity * article.sellPrice);
  }
  // Umsatzsteuerwert
  QString _vat_display;
  QString _vat_info(QString::number(article.vatValue));
  _vat_info.append("% " + tr("VAT"));
  if (article.vatSet == AntiquaCRM::SalesTax::TAX_INCL) {
    // Steuersatz ist im Artikel enthalten!
    double _v = inclVat(_price, article.vatValue);
    _vat_display = displayPrice(_v);
    _vat_info.prepend(tr("incl.") + " ");
    _total = _price;
  } else if (article.vatSet == AntiquaCRM::SalesTax::TAX_WITH) {
    // Steuersatz wird hinzugefügt!
    double _v = addVat(_price, article.vatValue);
    _total = (_price + _v);
    _vat_display = displayPrice(_v);
  } else {
    // Es wird keine Steuer angegeben!
    _vat_display = "-";
    _vat_info = tr("without sales tax");
    _total = _price;
  }

  // Preisausgabe im textformat
  QString _price_txt(QString::number(_price, 'f', 2));
  _price_txt.append(_currency);
  // aufrunden
  p_totalPrice += _total;

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << Qt::endl // INFO
           << "current price:" << _price << Qt::endl
           << "vat:" << _vat_info << _vat_display << Qt::endl
           << "article price:" << _price_txt << Qt::endl
           << "subtotal price:" << p_totalPrice << Qt::endl;
#endif
  // End:PrepareData

  QTextCursor cursor = body->textCursor();

  // Begin:Article
  QTextTableCell ce00 = table->cellAt(row, 0);
  ce00.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(article.articleid);

  QTextTableCell ce01 = table->cellAt(row, 1);
  ce01.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(article.designation);

  QTextTableCell ce02 = table->cellAt(row, 2);
  ce02.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(QString::number(article.quantity));

  QTextTableCell ce03 = table->cellAt(row, 3);
  ce03.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(_price_txt);
  // End:Article

  // BEGIN Mehwertsteuer
  row++;
  table->mergeCells(row, 0, 1, 3);
  QTextTableCell tc0 = table->cellAt(row, 0);
  tc0.setFormat(cellFormat(Printing::Border::NoBorder));
  cursor = tc0.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(_vat_info);
  QTextTableCell tc1 = table->cellAt(row, 3);
  tc1.setFormat(cellFormat(Printing::Border::NoBorder));
  cursor = tc1.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  if (article.vatDisabled) {
    cursor.insertText(displayPrice(0.00));
  } else {
    cursor.insertText(_vat_display);
  }
  // END

  // BEGIN Zwischensumme
  row++;
  table->mergeCells(row, 0, 1, 3);
  QTextTableCell zs0 = table->cellAt(row, 0);
  cursor = zs0.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(tr("Subtotal"));

  QTextTableCell zs1 = table->cellAt(row, 3);
  cursor = zs1.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.setBlockFormat(alignRight());
  cursor.insertText(displayPrice(p_totalPrice));
  // END

  return true;
}

void Printing::addPrinters() {
  int index = 0;
  selectPrinter->clear();
  selectPrinter->insertItem(index++, tr("Printers"));
  QListIterator<QPrinterInfo> it(QPrinterInfo::availablePrinters());
  while (it.hasNext()) {
    QPrinterInfo info = it.next();
    selectPrinter->insertItem(index, QIcon("://icons/printer.png"),
                              info.printerName(), info.isDefault());
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

const QPageSize Printing::pageSize() const {
  QPageSize init(QPageSize::A4);
  return QPageSize(init.definitionSize(), QPageSize::Point, init.name(),
                   QPageSize::ExactMatch);
}

const QPageLayout Printing::pageLayout() const {
  QPageLayout pageLayout;
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4), QMarginsF(0, 0, 0, 0));
  pageLayout.setMode(QPageLayout::StandardMode);
  pageLayout.setUnits(QPageLayout::Millimeter);
  return pageLayout;
}

const QPageLayout Printing::pdfLayout() const {
  QPageLayout pageLayout;
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4), QMarginsF(0, 0, 0, 0));
  pageLayout.setMode(QPageLayout::StandardMode);
  pageLayout.setUnits(QPageLayout::Millimeter);
  return pageLayout;
}

bool Printing::createPDF(const QString &section) {
  QPrinter *printer = new QPrinter(QPrinter::HighResolution);
  // printer->setResolution(72);
  printer->setPageLayout(pdfLayout());
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setCreator("AntiquaCRM");
  QString dest = outputDirectory(section);
  dest.append(QDir::separator());
  if (section == "invoices") {
    dest.append(p_invoiceId);
  } else if (section == "reminder") {
    dest.append(p_invoiceId);
  } else {
    dest.append(p_deliveryId);
  }
  dest.append(".pdf");
  printer->setOutputFileName(dest);
  return generateDocument(printer);
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
