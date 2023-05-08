// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printinvoice.h"

namespace AntiquaCRM {

InvoiceLetter::InvoiceLetter(AntiquaCRM::APrintingFormat *formatting,
                             QWidget *parent)
    : QWidget{parent}, format{formatting} {
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  const QRectF _rf = format->pointsRect();
  setFixedSize(_rf.width(), _rf.height());
}

const QStaticText InvoiceLetter::addressBlock(const QStringList &list) const {
  QTextOption _opt;
  _opt.setWrapMode(QTextOption::WordWrap);
  QStaticText _st;
  _st.setTextFormat(Qt::RichText);
  _st.setTextOption(_opt);
  _st.setText(list.join("<br>"));
  return _st;
}

void InvoiceLetter::paintEvent(QPaintEvent *) {
  const qreal _inlineWidth = format->inlineFrameWidth();
  const QRectF _addressFrame = format->letterWindowRect();
  QStringList _tmp_list;
  QFont _tmp_font(font());
  QTextOption _txt_options;
  _txt_options.setWrapMode(QTextOption::WordWrap);
  QPainter painter;
  painter.begin(this);
  painter.fillRect(rect(), Qt::white);

  // Letter folding lines
  int _yh = (rect().height() / 3);
  int _ym = (rect().height() / 2);
  painter.translate(0, 0);
  painter.setPen(QPen(Qt::gray));
  painter.drawLine(QPoint(5, _yh), // start
                   QPoint((format->borderLeft() / 2), _yh));
  painter.drawLine(QPoint(5, _ym), // start
                   QPoint(((format->borderLeft() / 3) * 2), _ym));
#ifdef ANTIQUA_DEVELOPEMENT
  // show borders
  painter.setPen(QPen(Qt::red));
  painter.drawLine(QPoint(format->borderLeft(), 0),
                   QPoint(format->borderLeft(), rect().height()));
  painter.drawLine(QPoint(format->borderRight(), 0),
                   QPoint(format->borderRight(), rect().height()));
  // Display letter window
  painter.drawRect(_addressFrame);
  painter.setPen(QPen(Qt::gray));
#endif

  // Start Document
  const QPointF _startPoint(format->borderLeft(), 0);
  painter.translate(_startPoint);
  painter.setPen(QPen(Qt::black));
  painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));

  // BEGIN:Heading
  qreal _posY = 0.0;
  const QImage image = format->watermark();
  if (!image.isNull()) {
    painter.setOpacity(format->watermarkOpacity());
    painter.drawImage(QPointF(5, 5), image);
    painter.setOpacity(1.0);
  }
  _tmp_font = format->getFont("print_font_header");
  _tmp_list = format->companyData("COMPANY_FULLNAME").split("#");
  const QString _header_title(_tmp_list.join("\n"));
  const QRectF _header_rect(0, _posY, _inlineWidth, _addressFrame.y());
  painter.setFont(_tmp_font);
  painter.drawText(_header_rect, Qt::AlignCenter, _header_title);
  // END:Heading

  // BEGIN:Address
  _tmp_list.clear();
  _tmp_list << format->companyData("COMPANY_SHORTNAME");
  _tmp_list << format->companyData("COMPANY_STREET");
  _tmp_list << format->companyData("COMPANY_LOCATION");
  _tmp_font = format->getFont("print_font_small");
  _tmp_font.setUnderline(true);
  qreal _padding = format->recipientPadding();
  const QString _addr_str = _tmp_list.join(" - ");
  const QFontMetricsF _addr_fm(_addr_str);
  _posY = (_addressFrame.y() - (_addr_fm.height() / 2));
  const QRectF _addr_rect(_padding, _posY,
                          _inlineWidth, // width
                          _addr_fm.height());

  painter.setFont(_tmp_font);
  painter.drawText(_addr_rect, Qt::AlignLeft, _addr_str);
  // Anschrift
  _tmp_list.clear();
  _tmp_list << "Hans Ulrich Mustermann";
  _tmp_list << "Langweiler Straße 105a";
  _tmp_list << "5689 Musterhausen";
  _tmp_list << "Germany";
  QStaticText recipient = addressBlock(_tmp_list);
  _tmp_font = format->getFont("print_font_normal");
  painter.setFont(_tmp_font);
  painter.drawStaticText(_padding, _addr_rect.y() + _padding, recipient);
  // END:Address

  // BEGIN:Subject

  // END:Subject

  painter.end();
}

PrintInvoice::PrintInvoice(QWidget *parent) : APrintDialog{parent} {}

bool PrintInvoice::generateDocument(QPrinter *printer) {
  Q_UNUSED(printer);
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
}

void PrintInvoice::openPrintDialog() { qDebug() << Q_FUNC_INFO << "TODO"; }

int PrintInvoice::exec(const QStringList &options) {
  Q_UNUSED(options);
  device = new InvoiceLetter(initFormatting(), this);
  device->update();
  setPrintingPage(device);

  return QDialog::exec();
}

} // namespace AntiquaCRM
