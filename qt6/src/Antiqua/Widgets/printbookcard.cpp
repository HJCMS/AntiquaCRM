// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printbookcard.h"

#include <QPrintDialog>

extern "C" {
#include "qrencode.h"
};

namespace AntiquaCRM {

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

BookCardPage::BookCardPage(const QJsonObject &data, QWidget *parent)
    : QWidget{parent}, p_data{data} {
  setObjectName("printing_bookcard_page");
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  QPalette _p = palette();
  _p.setColor(QPalette::Base, Qt::white);
  _p.setColor(QPalette::Text, Qt::black);
  setPalette(_p);

  const QRectF _prf = QPageSize(QPageSize::A6).rectPoints();
  setFixedSize(_prf.size().toSize());
}

void BookCardPage::paintEvent(QPaintEvent *) {
  QFontMetricsF _fmf(font());
  qreal _fontHeight = _fmf.height();
  static qreal _margin = 5;
  static qreal _padding = 2;

  QPainter painter;
  painter.begin(this);
  painter.fillRect(rect(), QBrush(Qt::white, Qt::SolidPattern));
  painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
  painter.setPen(Qt::black);

  QTextOption _textopts;
  _textopts.setWrapMode(QTextOption::WordWrap);

  QString _line = tr("Book Id") + ": " + p_data.value("article").toString();
  qreal _width = _fmf.horizontalAdvance(_line);
  qreal _ypos = (_fontHeight + _padding);
  painter.drawText((rect().width() - _width - _margin), _ypos, _line);

  _ypos += _padding;
  _line = p_data.value("name").toString();
  _line.append(" - ");
  _line.append(p_data.value("category").toString());
  QStaticText _text(_line);
  painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Compartment") + ": ";
  _line.append(p_data.value("compartment").toString());
  _text = QStaticText(_line);
  painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Title") + ": ";
  _line.append(p_data.value("title").toString());
  _text = QStaticText(_line);
  painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Author") + ": ";
  _line.append(p_data.value("author").toString());
  _text = QStaticText(_line);
  painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Year") + ": ";
  _line.append(p_data.value("year").toString());
  _text = QStaticText(_line);
  painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Changed") + ": ";
  _line.append(p_data.value("changed").toString());
  _text = QStaticText(_line);
  painter.drawStaticText(_margin, _ypos, _text);
  _ypos += (_text.size().height() + _padding);

  int _qrX = (((rect().width() / 2) - 64));
  int _qrY = (_ypos + (((rect().height() - _ypos) / 2) - 64));

  QUrl _url(p_data.value("qrquery").toString());
  BookCardQrCode bcCode(_url);
  adjustSize();
  QImage image = bcCode.image();
  if (!image.isNull()) {
    painter.drawImage(QPoint(_qrX, _qrY), image);
  }
  painter.end();
}

PrintBookCard::PrintBookCard(QWidget *parent)
    : AntiquaCRM::APrintDialog{parent} {
  setObjectName("print_bookcard_dialog");
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(QPageSize(QPageSize::A6));
  pageLayout.setMinimumMargins(QMargins(0, 0, 0, 0));
  pageLayout.setMargins(QMargins(0, 0, 0, 0));
  pageLayout.setUnits(QPageLayout::Millimeter);
  pageLayout.setMode(QPageLayout::FullPageMode);
}

void PrintBookCard::renderPage(QPrinter *printer) {
  Q_CHECK_PTR(page);
  QPainter painter(printer);
  painter.setWindow(page->rect());
  painter.translate(0, 0);
  page->render(&painter);
  painter.end();
}

void PrintBookCard::createPDF() {
  QDir _dir(config->value("dirs/archive_cards").toString());
  if (_dir.exists()) {
    QFileInfo _file(_dir, pdfFileName);
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageLayout(pageLayout);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setCreator("AntiquaCRM");
    printer->setOutputFileName(_file.filePath());
    renderPage(printer);
    if (_file.isReadable())
      sendStatusMessage(tr("PDF Document created!"));
  }
}

void PrintBookCard::openPrintDialog() {
  QPrinterInfo p_info;
  QString _a6_printer = config->value("printer/device_secondary").toString();
  if (_a6_printer.isEmpty()) {
    p_info = QPrinterInfo::defaultPrinter();
  } else {
    p_info = QPrinterInfo::printerInfo(_a6_printer);
  }

  QPrinter *printer = new QPrinter(p_info, QPrinter::ScreenResolution);
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPageLayout(pageLayout);
  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setDocName(pdfFileName);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setPrintRange(QAbstractPrintDialog::CurrentPage);
  dialog->setOption(QAbstractPrintDialog::PrintShowPageSize, true);

  connect(dialog, SIGNAL(accepted(QPrinter *)), SLOT(renderPage(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    done(QDialog::Accepted);
    sendStatusMessage(tr("Bookcard printed!"));
  }
}

int PrintBookCard::exec(const QJsonObject &options) {
  if (!options.contains("article"))
    return QDialog::Rejected;

  pdfFileName = options.value("article").toString();
  pdfFileName.append(".pdf");

  page = new BookCardPage(options, this);
  viewPort->setWidget(page);
  update();

  return QDialog::exec();
}

} // namespace AntiquaCRM
