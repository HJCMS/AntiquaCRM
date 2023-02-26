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

  QScrollArea *scrollAera = new QScrollArea(this);
  scrollAera->setObjectName("scroll_area");
  scrollAera->setWidgetResizable(true);
  layout->addWidget(scrollAera);

  printArea = new QWidget(scrollAera);
  printArea->setObjectName("printing_area");
  printArea->setContentsMargins(0, 0, 0, 0);
  printArea->setStyleSheet("border:none;");

  QRectF pageRect = QPageSize(QPageSize::A4).rect(QPageSize::Point);
  printArea->setFixedSize(pageRect.toRect().size());

  QVBoxLayout *frame_layout = new QVBoxLayout(printArea);
  frame_layout->setContentsMargins(0, 0, 0, 0);
  frame_layout->setSpacing(0);

  header = new TextEditor(printArea);
  header->setObjectName("printing_header");
  header->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(header);

  body = new TextEditor(printArea);
  body->setObjectName("printing_body");
  frame_layout->addWidget(body);

  footer = new TextEditor(printArea);
  footer->setObjectName("printing_footer");
  footer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(footer);

  frame_layout->setStretchFactor(header, 25);
  frame_layout->setStretchFactor(body, 50);
  frame_layout->setStretchFactor(footer, 15);
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

  config->beginGroup("printer/table_margins");
  QMarginsF fm(20.0, 1.0, 20.0, 1.0);
  p_margins.setLeft(config->value("left", fm.left()).toReal());
  p_margins.setTop(config->value("top", fm.top()).toReal());
  p_margins.setRight(config->value("right", fm.right()).toReal());
  p_margins.setBottom(config->value("bottom", fm.bottom()).toReal());
  config->endGroup();

  config->beginGroup("printer");

  // Position wann adressenkopf begint!
  p_subjectPosition = config->value("subject_position", 120).toInt();
  // Wasserzeichen
  p_watermark_opacity = config->value("watermark_opacity", 1.0).toReal();

  QFont font;
  if (font.fromString(config->value("header_font").toString())) {
    headerFont.swap(font);
  }
  if (font.fromString(config->value("address_font").toString())) {
    addressFont.swap(font);
  }
  if (font.fromString(config->value("subject_font").toString())) {
    subjectFont.swap(font);
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

const QPen Printing::penStyle() const {
  QPen pen(Qt::gray, Qt::SolidLine);
  pen.setCapStyle(Qt::FlatCap);
  pen.setWidthF(0.6);
  pen.setCosmetic(true);
  return pen;
}

const QTextCharFormat Printing::charFormat(const QFont &f, bool bolded) {
  QTextCharFormat format;

  QFont font(f);
  font.setBold(bolded);

  format.setFont(font);
  return format;
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
  f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  f.setPadding(0);
  f.setCellPadding(0);
  f.setCellSpacing(0);
  f.setTopMargin(0);
  f.setBottomMargin(0);
  f.setLeftMargin(p_margins.left());
  f.setRightMargin(p_margins.right());
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
  return QBrush(Qt::gray, Qt::SolidPattern);
}

QTextFrame *Printing::bodyFrame() {
  QTextFrame *frame = new QTextFrame(body->document());
  return frame;
}

void Printing::constructHeader() {
  QTextBlockFormat block;
  block.setAlignment(Qt::AlignCenter);
  QTextCursor cursor = header->textCursor();
  cursor.setCharFormat(charFormat(getHeaderFont()));
  QString title = companyData.value("COMPANY_PRINTING_HEADER");
  foreach (QString line, title.split("\n")) {
    cursor.insertBlock(block);
    cursor.insertText(line);
    cursor.atEnd();
  }
  QFontMetricsF fm(getHeaderFont());
  int w = header->size().width();
  int h = qRound(fm.height() * (header->document()->lineCount() + 1));
  header->resize(QSize(w, h));
  header->update();
  header->setMaximumHeight(h);
  header->setReadOnly(true);
  header->document()->setModified(true);
}

QTextTable *Printing::constructSubjectTable(const QString &subject) {
  QTextCursor cursor = body->textCursor();
  QTextTable *table = cursor.insertTable(2, 2, tableFormat());

  QTextCharFormat cellFormat;
  QFont font(getSmallFont());
  font.setUnderline(true);
  font.setPointSize(8);
  cellFormat.setFont(font);
  cellFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);

  QTextTableCell tc00 = table->cellAt(0, 0);
  tc00.setFormat(cellFormat);
  cursor = tc00.firstCursorPosition();
  QString addr(companyData.value("COMPANY_SHORTNAME"));
  addr.append(" - ");
  addr.append(companyData.value("COMPANY_STREET"));
  addr.append(" - ");
  addr.append(companyData.value("COMPANY_LOCATION"));
  cursor.insertText(addr);

  // Subject
  QTextTableCell tc01 = table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(subject);

  return table;
}

void Printing::constructFooter() {
  QTextCursor cursor = footer->textCursor();
  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setBorderBrush(borderBrush());
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  // FOOTER
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  QTextTable *table = cursor.insertTable(1, 2, format);

  QTextTableCell ce00 = table->cellAt(0, 0);
  ce00.setFormat(cellFormat);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(charFormat(getFooterFont()));
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
  cursor.setCharFormat(charFormat(getFooterFont()));
  cursor.insertText(companyData.value("COMPANY_BANK_NAME") + "\n");
  cursor.insertText("Swift-BIC: ");
  cursor.insertText(companyData.value("COMPANY_BANK_BICSWIFT") + "\n");
  cursor.insertText("IBAN: ");
  cursor.insertText(companyData.value("COMPANY_BANK_IBAN") + "\n");
  cursor.insertText(tr("Tax No.") + ": ");
  cursor.insertText(companyData.value("COMPANY_VAT_NUMBER") + "\n");
  cursor.insertText(tr("EPR No.") + ": ");
  cursor.insertText(companyData.value("COMPANY_EPR_NUMBER") + "\n");
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

void Printing::setRegards(const QStringList &list) {
  QTextCursor cursor = body->textCursor();
  QTextBlockFormat bf;
  bf.setLeftMargin(p_margins.left());
  cursor.setBlockFormat(bf);
  cursor.setCharFormat(charFormat(getFooterFont()));
  cursor.insertText("\n\n");
  foreach (QString l, list) {
    cursor.insertText(l + "\n");
  }
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
    _vat_display = money(_v);
    _vat_info.prepend(tr("incl.") + " ");
    _total = _price;
  } else if (article.vatSet == AntiquaCRM::SalesTax::TAX_WITH) {
    // Steuersatz wird hinzugefügt!
    double _v = addVat(_price, article.vatValue);
    _total = (_price + _v);
    _vat_display = money(_v);
  } else {
    // Es wird keine Steuer angegeben!
    _vat_display = "-";
    _vat_info = tr("without sales tax");
    _total = _price;
  }

  // Preisausgabe im textformat
  QString _price_txt = money(_price);
  // aufrunden
  p_totalPrice += _total;
  // End:PrepareData

  QTextCursor cursor = body->textCursor();

  // Begin:Article
  QTextTableCell ce00 = table->cellAt(row, 0);
  ce00.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.insertText(article.articleid);

  QTextTableCell ce01 = table->cellAt(row, 1);
  ce01.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.insertText(article.designation);

  QTextTableCell ce02 = table->cellAt(row, 2);
  ce02.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.setBlockFormat(alignCenter());
  cursor.insertText(QString::number(article.quantity));

  QTextTableCell ce03 = table->cellAt(row, 3);
  ce03.setFormat(cellFormat(Printing::Border::Top));
  cursor = ce03.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.setBlockFormat(alignRight());
  cursor.insertText(_price_txt);
  // End:Article

  // BEGIN Mehwertsteuer
  row++;
  table->mergeCells(row, 0, 1, 3);
  QTextTableCell tc0 = table->cellAt(row, 0);
  tc0.setFormat(cellFormat(Printing::Border::NoBorder));
  cursor = tc0.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.setBlockFormat(alignRight());
  cursor.insertText(_vat_info);
  QTextTableCell tc1 = table->cellAt(row, 3);
  tc1.setFormat(cellFormat(Printing::Border::NoBorder));
  cursor = tc1.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  if (article.vatDisabled) {
    cursor.setBlockFormat(alignCenter());
    cursor.insertText("-");
  } else {
    cursor.setBlockFormat(alignRight());
    cursor.insertText(_vat_display);
  }
  // END

  // BEGIN Zwischensumme
  row++;
  table->mergeCells(row, 0, 1, 3);
  QTextTableCell zs0 = table->cellAt(row, 0);
  cursor = zs0.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.setBlockFormat(alignRight());
  cursor.insertText(tr("Subtotal"));

  QTextTableCell zs1 = table->cellAt(row, 3);
  cursor = zs1.firstCursorPosition();
  cursor.setCharFormat(charFormat(getNormalFont()));
  cursor.setBlockFormat(alignRight());
  cursor.insertText(money(p_totalPrice));
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

const QString Printing::money(double v) const {
  QLocale lc = QLocale::system();
  QString cs = lc.currencySymbol(QLocale::CurrencySymbol);
  return lc.toCurrencyString(v, cs, 2);
}

const QPageSize Printing::pageSize() const {
  QPageSize init(QPageSize::A4);
  return QPageSize(init.definitionSize(), QPageSize::Point, init.name(),
                   QPageSize::ExactMatch);
}

const QPageLayout Printing::pageLayout() const {
  QPageLayout pageLayout;
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A4));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(p_margins);
  pageLayout.setMode(QPageLayout::FullPageMode);
  return pageLayout;
}

bool Printing::createPDF(const QString &section) {
  QPrinter *printer = new QPrinter(QPrinter::HighResolution);
  printer->setPageLayout(pageLayout());
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

const QFont Printing::getHeaderFont() { return headerFont; }

const QFont Printing::getAddressFont() { return addressFont; }

const QFont Printing::getNormalFont() { return normalFont; }

const QFont Printing::getFooterFont() { return footerFont; }

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
