// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookcard.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "printing.h"

#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QStyle>
#include <QtWidgets>

BookCardPaintWidget::BookCardPaintWidget(QWidget *parent)
    : QWidget{parent}, m_qrCode(QtQrCode::StringMode),
      m_qrCodePainter(1, QBrush(Qt::white, Qt::SolidPattern),
                      QBrush(Qt::black, Qt::SolidPattern)) {
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

void BookCardPaintWidget::paintEvent(QPaintEvent *p) {
  QRect r = p->rect();
  int w = p->rect().width();
  int h = p->rect().height();
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

  m_qrCode.setData(p_queryUrl.toEncoded(QUrl::None));
  QImage image = m_qrCodePainter.toImage(m_qrCode, qr_size.height());

  int _x = (w - qr_size.height()) / 2;
  int _y = yPos + (((h - yPos) / 2) - (qr_size.height() / 2));
  painter.drawImage(QPointF(_x, _y), image);

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

  page_size = QPageSize(QPageSize::A6);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_card_layout");

  QRect pageRect = pageLayout().fullRectPoints();
  int maxWidth = pageRect.width();

  m_card = new BookCardPaintWidget(this);
  m_card->setObjectName("printing_header");
  m_card->setStyleSheet("border:none;color:black;");
  m_card->setFixedSize(pageRect.size());
  m_card->setContentsMargins(0, 0, 0, 0);
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
  pageLayout.setPageSize(page_size, QMarginsF(2, 2, 2, 2));
  pageLayout.setMode(QPageLayout::FullPageMode);
  pageLayout.setUnits(QPageLayout::Millimeter);
  return pageLayout;
}

bool BookCard::createPDF() {
  QPrinter *printer = new QPrinter(QPrinter::PrinterResolution);
  QString dest = p_destination;
  dest.append(QDir::separator());
  dest.append(p_filename);
  dest.append(".pdf");
  printer->setOutputFileName(dest);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setPageLayout(pageLayout());
  return printDocument(printer);
}

bool BookCard::printDocument(QPrinter *printer) {
  QPageLayout layout = pageLayout();
  printer->setPageLayout(layout);
  QPainter painter(printer);
  painter.translate(0, 0);
  m_card->render(&painter);
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

  QPrinter *printer = new QPrinter(QPrinter::PrinterResolution);
  printer->setPageLayout(pageLayout());
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPrinterName(p_printerName);

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
