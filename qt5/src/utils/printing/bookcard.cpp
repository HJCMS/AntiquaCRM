// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookcard.h"

#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinterInfo>
#include <QStyle>
#include <QtWidgets>

BookCardQrCode::BookCardQrCode(const QUrl &url, int size)
    : p_url(url), p_size(size) {}

const QImage BookCardQrCode::image() {
  QImage image = QImage();
  if (!p_url.isValid())
    return image;

  QByteArray array = p_url.toEncoded(QUrl::None);
  std::string str = array.toStdString();

  QRcode *code = QRcode_encodeString(str.c_str(),  /**< data. */
                                     0,            /**< version */
                                     QR_ECLEVEL_L, /**< level */
                                     QR_MODE_8,    /**< UTF-8 */
                                     1             /**< sensitive */
  );

  image = QImage(code->width + 8, code->width + 8, QImage::Format_RGB32);
  image.fill(0xffffff);
  unsigned char *px = code->data;
  for (int y = 0; y < code->width; y++) {
    for (int x = 0; x < code->width; x++) {
      image.setPixel(x, y, ((*px & 0x1) ? 0x000000 : 0xffffff));
      px++;
    }
  }
  QRcode_free(code);

  return image.scaled(p_size, p_size);
}

BookCardPaintWidget::BookCardPaintWidget(QWidget *parent) : QWidget{parent} {
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

void BookCardPaintWidget::paintEvent(QPaintEvent *) {
  QRect r = rect();
  int w = r.width();
  int h = r.height();
  int margin = 5;
  QFontMetricsF fm(font());
  qreal fontHeight = fm.height();
  qreal yPos = 0;

  QTextOption textOptions;
  textOptions.setWrapMode(QTextOption::WordWrap);

  QPainter painter;
  painter.begin(this);
  painter.fillRect(r, Qt::white);
  painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));

  qreal txt_width = fm.horizontalAdvance(p_id);
  yPos = (fontHeight + margin);
  painter.drawText((w - txt_width - margin), yPos, p_id);

  yPos += margin;
  QStaticText storage(p_storage);
  painter.drawStaticText(margin, yPos, storage);

  yPos += (fontHeight + margin);
  QStaticText txtAuthor(p_author);
  txtAuthor.setTextFormat(Qt::PlainText);
  txtAuthor.setTextWidth((w - (margin * 2)));
  painter.drawStaticText(margin, yPos, txtAuthor);
  yPos += (txtAuthor.size().height() + margin);

  QStaticText txtKeywords(p_keywords.replace(",", ", "));
  txtKeywords.setTextFormat(Qt::PlainText);
  txtKeywords.setTextOption(textOptions);
  txtKeywords.setTextWidth((w - (margin * 2)));
  painter.drawStaticText(margin, yPos, txtKeywords);
  yPos += (txtKeywords.size().height() + margin);

  QStaticText txtTitle(p_title);
  txtTitle.setTextFormat(Qt::PlainText);
  txtTitle.setTextWidth((w - (margin * 2)));
  txtTitle.setTextOption(textOptions);
  painter.drawStaticText(margin, yPos, txtTitle);
  yPos += (txtTitle.size().height() + margin);

  QStaticText txtYear(p_year);
  txtYear.setTextFormat(Qt::PlainText);
  txtYear.setTextWidth((w - (margin * 2)));
  painter.drawStaticText(margin, yPos, txtYear);
  yPos += (txtYear.size().height() + margin);

  QStaticText txtSince(p_since);
  txtSince.setTextFormat(Qt::PlainText);
  txtSince.setTextWidth((w - (margin * 2)));
  painter.drawStaticText(margin, yPos, txtSince);
  yPos += (txtSince.size().height() + margin);

  painter.drawLine(0, yPos, w, yPos);

  yPos += 20;
  int _size = qMin(qRound((h - yPos) - (margin * 2.0)), /* resthöhe */
                   qRound(w - (margin * 2.0)));

  int _x = ((w / 2) - (_size / 2));
  BookCardQrCode bcCode(p_queryUrl, ((_size > 128) ? 128 : _size));
  QImage image = bcCode.image();
  if (!image.isNull()) {
    painter.drawImage(QPoint(_x, yPos), image);
  }
  painter.end();
}

void BookCardPaintWidget::setQrUrl(const QUrl &url) { p_queryUrl = url; }

void BookCardPaintWidget::setArticleId(const QString &txt) {
  p_id = tr("Article Nr.");
  p_id.append(": ");
  p_id.append(txt);
}

void BookCardPaintWidget::setStorage(int id) {
  QString key("storagelocations");
  AntiquaCRM::ASharedDataFiles dataFile;
  if (dataFile.fileExists(key)) {
    QJsonDocument jdoc = dataFile.getJson(key);
    QJsonArray arr = jdoc.object().value(key).toArray();
    if (arr.size() > 0) {
      for (int i = 0; i < arr.size(); i++) {
        QJsonObject jo = arr[i].toObject();
        if (jo.value("id").toInt() == id) {
          p_storage = jo.value("storage").toString();
          p_storage.append(" - " + jo.value("identifier").toString());
          break;
        }
      }
    }
  } else {
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QString sql("SELECT sl_storage,sl_identifier ");
    sql.append("FROM ref_storage_location ORDER BY sl_id;");
    QSqlQuery q = m_sql->query(sql);
    if (q.size() > 0) {
      while (q.next()) {
        if (q.value("sl_id").toInt() == id) {
          p_storage = q.value("sl_storage").toString();
          p_storage.append(" - " + q.value("sl_identifier").toString());
          break;
        }
      }
    }
  }
}

void BookCardPaintWidget::setDescription(
    const QHash<QString, QVariant> &list) {
  p_title = tr("Title") + ": " + list.value("title").toString();
  p_author = tr("Author") + ": " + list.value("author").toString();
  p_keywords = tr("Keywords") + ": " + list.value("keywords").toString();
  p_year = tr("Year") + ": " + list.value("year").toString();
  QDate date = list.value("since").toDate();
  p_since = tr("Registered") + ": " + date.toString();
}

BookCardConfig::BookCardConfig(QWidget *parent) : QWidget{parent} {
  QGridLayout *layout = new QGridLayout(this);
  int row = 0;

  layout->addWidget(new QLabel(tr("Printer"), this), row, 0, 1, 1,
                    Qt::AlignRight);

  m_printer = new QComboBox(this);
  QList<QPrinterInfo> pList = QPrinterInfo::availablePrinters();
  int index = 0;
  for (int i = 0; i < pList.count(); i++) {
    QPrinterInfo pi = pList.at(i);
    m_printer->insertItem(index, pi.printerName(), Qt::DisplayRole);
    m_printer->insertItem(index, pi.description(), Qt::UserRole);
    index++;
  }
  layout->addWidget(m_printer, row++, 1, 1, 1);

  layout->addWidget(new QLabel(tr("Paper Feed"), this), row, 0, 1, 1,
                    Qt::AlignRight);
  m_printerPaperFeed = new QComboBox(this);
  m_printerPaperFeed->addItem(tr("Change Paper Feed not supported"), -1);
  m_printerPaperFeed->addItem(tr("Auto"), QPrinter::Auto);
  m_printerPaperFeed->addItem(tr("Cassette"), QPrinter::Cassette);
  m_printerPaperFeed->addItem(tr("Envelope"), QPrinter::Envelope);
  m_printerPaperFeed->addItem(tr("Envelope Manual"), QPrinter::EnvelopeManual);
  m_printerPaperFeed->addItem(tr("Source"), QPrinter::FormSource);
  m_printerPaperFeed->addItem(tr("Large Capacity"), QPrinter::LargeCapacity);
  m_printerPaperFeed->addItem(tr("Large Format"), QPrinter::LargeFormat);
  m_printerPaperFeed->addItem(tr("Lower"), QPrinter::Lower);
  m_printerPaperFeed->addItem(tr("Middle"), QPrinter::Middle);
  m_printerPaperFeed->addItem(tr("Manual"), QPrinter::Manual);
  m_printerPaperFeed->addItem(tr("Only one"), QPrinter::OnlyOne);
  m_printerPaperFeed->addItem(tr("Tractor"), QPrinter::Tractor);
  m_printerPaperFeed->addItem(tr("Small Format"), QPrinter::SmallFormat);
  m_printerPaperFeed->addItem(tr("Custom Source"), QPrinter::CustomSource);
  m_printerPaperFeed->setEnabled(false);
  layout->addWidget(m_printerPaperFeed, row++, 1, 1, 1);

  m_info = new QLabel(this);
  layout->addWidget(m_info, row++, 0, 1, 2);

  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(m_printer, SIGNAL(currentIndexChanged(int)), this,
          SLOT(infoPrinterChanged(int)));
  connect(m_printerPaperFeed, SIGNAL(currentIndexChanged(int)), this,
          SLOT(paperSourceChanged(int)));
}

void BookCardConfig::infoPrinterChanged(int index) {
  QString name = m_printer->itemData(index, Qt::DisplayRole).toString();
  if (name.isEmpty())
    emit s_printerChanged(name);
}

void BookCardConfig::paperSourceChanged(int index) {
  int s = m_printerPaperFeed->itemData(index, Qt::UserRole).toInt();
  QPrinter::PaperSource ps = static_cast<QPrinter::PaperSource>(s);
  emit s_paperSourceChanged(ps);
}

void BookCardConfig::setPrinterByName(const QString &name) {
  int index = m_printer->findData(name, Qt::DisplayRole);
  if (index != m_printer->currentIndex())
    m_printer->setCurrentIndex(index);
}

void BookCardConfig::printerChanged(QPrinter *printer) {
  for (int i = 0; i < m_printerPaperFeed->count(); i++) {
    int ps = m_printerPaperFeed->itemData(i, Qt::UserRole).toInt();
    if (printer->paperSource() == static_cast<QPrinter::PaperSource>(ps)) {
      m_printerPaperFeed->setCurrentIndex(i);
      m_printerPaperFeed->setEnabled(true);
    }
  }
#ifdef Q_OS_WIN
  QList<QPrinter::PaperSource> psl = printer->supportedPaperSources();
  m_printerPaperFeed->setEnabled((psl.count() >= 1));
#endif
}

BookCard::BookCard(QWidget *parent) : QDialog{parent} {
  setObjectName("printing_article_card");
  setWindowTitle(tr("Printing article card"));
  setSizeGripEnabled(false);

  p_paperSource = QPrinter::OnlyOne;

  config = new AntiquaCRM::ASettings(this);

  QGridLayout *layout = new QGridLayout(this);
  layout->setObjectName("printing_card_layout");
  layout->setSizeConstraint(QLayout::SetFixedSize);

  QRect pageRect = pageLayout().fullRectPoints();
  m_card = new BookCardPaintWidget(this);
  m_card->setObjectName("print_card_render");
  m_card->setStyleSheet("border:none;color:black;");
  m_card->setContentsMargins(0, 0, 0, 0);
  m_card->setFixedSize(pageRect.size());
  m_card->setWindowModified(true);
  layout->addWidget(m_card, 0, 0, 1, 1);

  m_cardConfig = new BookCardConfig(this);
  m_cardConfig->setObjectName("print_card_config");
  layout->addWidget(m_cardConfig, 0, 1, 1, 1);

  QDialogButtonBox *btn_box = new QDialogButtonBox(this);
  btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Close);
  layout->addWidget(btn_box, 1, 0, 1, 2);

  setLayout(layout);
  connect(btn_box, SIGNAL(accepted()), this, SLOT(openPrintDialog()));
  connect(btn_box, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_cardConfig, SIGNAL(s_printerChanged(const QString &)),
          SLOT(updatePrinterByName(const QString &)));
  connect(m_cardConfig, SIGNAL(s_paperSourceChanged(QPrinter::PaperSource)),
          SLOT(updatePaperSource(QPrinter::PaperSource)));
}

void BookCard::readConfiguration() {
  config->beginGroup("printer");
  QFont font = m_card->font();
  if (font.fromString(config->value("normal_font").toString())) {
    m_card->setFont(font);
  }
  p_printerName = config->value("DIN_A6_Printer").toString();
  config->endGroup();
  p_destination = config->value("dirs/cards").toString();
}

const QUrl BookCard::generateQRCodeUrl() {
  QUrl url;
  config->beginGroup("qrcode");
  url.setScheme(config->value("schema", "https").toString());
  url.setHost(config->value("server", "localhost").toString());
  url.setPath(config->value("path", "/").toString());
  QString query(config->value("parameter", "articleid").toString());
  query.append("=");
  query.append(QString::number(p_articleId));
  url.setQuery(query);
  config->endGroup();
  return url;
}

const QPageLayout BookCard::pageLayout() {
  QPageLayout pageLayout;
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A6), QMarginsF(2, 2, 2, 2));
  pageLayout.setMode(QPageLayout::FullPageMode);
  pageLayout.setUnits(QPageLayout::Millimeter);
  return pageLayout;
}

bool BookCard::createPDF() {
  QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout());
  printer->setOutputFormat(QPrinter::PdfFormat);
  QString dest = p_destination;
  dest.append(QDir::separator());
  dest.append(p_filename);
  dest.append(".pdf");
  printer->setOutputFileName(dest);
  printer->setCreator("AntiquaCRM");
  return printDocument(printer);
}

void BookCard::updatePrinterByName(const QString &name) {
  p_printerName = name;
}

void BookCard::updatePaperSource(QPrinter::PaperSource ps) {
  p_paperSource = ps;
}

bool BookCard::printDocument(QPrinter *printer) {
  printer->setPageLayout(pageLayout());
  m_cardConfig->printerChanged(printer);
  QPainter painter(printer);
  painter.setWindow(m_card->rect());
  painter.translate(0, 0);
  m_card->render(&painter);
  painter.end();
  return true;
}

void BookCard::openPrintDialog() {
  if (createPDF()) {
    qInfo("PDF File written.");
  } else {
    qWarning("PDF not generated");
  }

  if (p_printerName.isEmpty()) {
    qWarning("No Printer found");
    return;
  }

  QPrinterInfo p_info = QPrinterInfo::printerInfo(p_printerName);
  QPrinter *printer = new QPrinter(p_info, QPrinter::ScreenResolution);
  printer->setPageLayout(pageLayout());
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPrinterName(p_printerName);
  printer->setDocName(p_filename);
  printer->setCreator("AntiquaCRM");
  m_cardConfig->printerChanged(printer);
#ifdef Q_OS_WIN
  printer->setPaperSource(p_paperSource);
#endif

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)),
          SLOT(printDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
}

int BookCard::exec() {
  qWarning("you must call exec with params!");
  return QDialog::Rejected;
}

int BookCard::exec(const QHash<QString, QVariant> &data) {
  readConfiguration();

  if (data.count() < 1) {
    qWarning("Invalid data count for Card printing!");
    return QDialog::Rejected;
  }

  p_articleId = data.value("id").toInt();
  if (p_articleId < 1) {
    qWarning("Invalid Article ID!");
    return QDialog::Rejected;
  }

  QString id = QString::number(p_articleId);
  m_card->setArticleId(id);
  m_card->setQrUrl(generateQRCodeUrl());

  p_filename = id;
  m_card->setDescription(data);
  m_card->setStorage(data.value("storage").toInt());

  return QDialog::exec();
}
