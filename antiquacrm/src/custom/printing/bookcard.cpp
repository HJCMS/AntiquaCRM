// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookcard.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "printing.h"

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

  QRcode *code =
      QRcode_encodeString(str.c_str(),  /**< must be NUL terminated. */
                          0,            /**< version */
                          QR_ECLEVEL_L, /**< error correction level */
                          QR_MODE_8,    /**< If you want to embed UTF-8 */
                          1             /**< case-sensitive */
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

void BookCardPaintWidget::paintEvent(QPaintEvent *p) {
  QRect r = rect();
  int w = r.width();
  int h = r.height();
  int margin = 5;
  QFontMetricsF fm(font());
  qreal fontHeight = fm.height();
  qreal yPos = 0;

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

  QStaticText txtTitle(p_title);
  txtTitle.setTextFormat(Qt::PlainText);
  txtTitle.setTextWidth((w - (margin * 2)));
  painter.drawStaticText(margin, yPos, txtTitle);
  yPos += (txtTitle.size().height() + margin);

  QStaticText txtYear(p_year);
  txtYear.setTextFormat(Qt::PlainText);
  txtYear.setTextWidth((w - (margin * 2)));
  painter.drawStaticText(margin, yPos, txtYear);
  yPos += (txtYear.size().height() + margin);

  painter.drawLine(0, yPos, w, yPos);

  yPos += 20;
  int _size = qMin(qRound((h - yPos) - (margin * 2.0)), /* resthöhe */
                   qRound(w - (margin * 2.0)));

  int _x = ((w / 2) - (_size / 2));
  BookCardQrCode bcCode(p_queryUrl, _size);
  QImage image = bcCode.image();
  if (!image.isNull()) {
    painter.drawImage(QPoint(_x, yPos), image);
  }
  painter.end();
}

void BookCardPaintWidget::setQrUrl(const QUrl &url) { p_queryUrl = url; }

void BookCardPaintWidget::setArticleId(const QString &txt) {
  p_id = tr("Book Nr.");
  p_id.append(": ");
  p_id.append(txt);
}

void BookCardPaintWidget::setStorage(const QString &txt) { p_storage = txt; }

void BookCardPaintWidget::setBookDescription(
    const QHash<QString, QVariant> &list) {
  p_title = tr("Title") + ": " + list.value("title").toString();
  p_author = tr("Author") + ": " + list.value("author").toString();
  p_year = tr("Year") + ": " + list.value("year").toString();
}

BookCard::BookCard(QWidget *parent) : QDialog{parent} {
  setObjectName("printing_book_card");
  setWindowTitle(tr("Printing book card"));
  setSizeGripEnabled(false);

  config = new ApplSettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_card_layout");

  QRect pageRect = pageLayout().fullRectPoints();
  m_card = new BookCardPaintWidget(this);
  m_card->setObjectName("printing_header");
  m_card->setStyleSheet("border:none;color:black;");
  m_card->setContentsMargins(0, 0, 0, 0);
  m_card->setFixedSize(pageRect.size());
  m_card->setWindowModified(true);
  layout->addWidget(m_card);

  QDialogButtonBox *btn_box = new QDialogButtonBox(this);
  btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Close);
  layout->addWidget(btn_box);

  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);
  connect(btn_box, SIGNAL(accepted()), this, SLOT(openPrintDialog()));
  connect(btn_box, SIGNAL(rejected()), this, SLOT(reject()));
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

bool BookCard::printDocument(QPrinter *printer) {
  printer->setPageLayout(pageLayout());
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

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  connect(dialog, SIGNAL(accepted(QPrinter *)), this,
          SLOT(printDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
  }
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
  m_card->setBookDescription(data);
  m_card->setStorage(data.value("storage").toString());

  return QDialog::exec();
}
