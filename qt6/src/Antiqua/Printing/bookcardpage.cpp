// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookcardpage.h"
#include "bookcardqrcode.h"

#include <QBrush>
#include <QFontMetricsF>
#include <QPageSize>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QRectF>
#include <QStaticText>
#include <QTextOption>

namespace AntiquaCRM {

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

  QPainter _painter;
  _painter.begin(this);
  _painter.fillRect(rect(), QBrush(Qt::white, Qt::SolidPattern));
  _painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
  _painter.setPen(Qt::black);

  QTextOption _textopts;
  _textopts.setWrapMode(QTextOption::WordWrap);

  QString _line = tr("Book Id") + ": " + p_data.value("article").toString();
  qreal _width = _fmf.horizontalAdvance(_line);
  qreal _ypos = (_fontHeight + _padding);
  _painter.drawText((rect().width() - _width - _margin), _ypos, _line);

  _ypos += _padding;
  _line = p_data.value("name").toString();
  _line.append(" - ");
  _line.append(p_data.value("category").toString());
  QStaticText _text(_line);
  _painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Compartment") + ": ";
  _line.append(p_data.value("compartment").toString());
  _text = QStaticText(_line);
  _painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Title") + ": ";
  _line.append(p_data.value("title").toString());
  _text = QStaticText(_line);
  _painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Author") + ": ";
  _line.append(p_data.value("author").toString());
  _text = QStaticText(_line);
  _painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Year") + ": ";
  _line.append(p_data.value("year").toString());
  _text = QStaticText(_line);
  _painter.drawStaticText(_margin, _ypos, _text);

  _ypos += (_text.size().height() + _padding);
  _line = tr("Changed") + ": ";
  _line.append(p_data.value("changed").toString());
  _text = QStaticText(_line);
  _painter.drawStaticText(_margin, _ypos, _text);
  _ypos += (_text.size().height() + _padding);

  int _qrX = (((rect().width() / 2) - 64));
  int _qrY = (_ypos + (((rect().height() - _ypos) / 2) - 64));

  QUrl _url(p_data.value("qrquery").toString());
  BookCardQrCode bcCode(_url);
  adjustSize();
  QImage image = bcCode.image();
  if (!image.isNull()) {
    _painter.drawImage(QPoint(_qrX, _qrY), image);
  }
  _painter.end();
}

} // namespace AntiquaCRM
