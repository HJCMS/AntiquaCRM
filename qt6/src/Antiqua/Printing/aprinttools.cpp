// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprinttools.h"

#include <QRegularExpression>

namespace AntiquaCRM {

APrintTools::APrintTools(QPageSize::PageSizeId id) : pageSize{id} {}

qreal APrintTools::getPoints(int millimeter) const {
  return qRound(millimeter * points);
}

const QStaticText APrintTools::textBlock(Qt::Alignment align,
                                         QTextOption::WrapMode mode,
                                         Qt::TextFormat format) const {
  QTextOption _opts(align);
  _opts.setWrapMode(mode);

  QStaticText _box;
  _box.setTextFormat(format);
  _box.setTextOption(_opts);
  return _box;
}

const QString APrintTools::toRichText(const QString &txt) const {
  static const QRegularExpression pattern("[\\n\\r]+",
                                          QRegularExpression::NoPatternOption);
  QString _txt(txt.trimmed());
  _txt.replace(pattern, "<br>");
  return _txt;
}

const QRectF APrintTools::pagePoints() const {
  const QRectF _lr = pageLayout().fullRect(QPageLayout::Millimeter);
  return QRectF(0, 0, (_lr.width() * points), (_lr.height() * points));
}

const QRectF APrintTools::letterWindow(qreal left) const {
  // https://de.wikipedia.org/wiki/DIN_5008
  return QRectF(getPoints(left), // 25mm left
                getPoints(45),   // 45mm top
                getPoints(85),   // 85mm width
                getPoints(45)    // 45mm height
  );
}

const QRectF APrintTools::addressZone() const {
  return QRectF(letterWindow().left() + getPoints(5), // +5mm left
                letterWindow().top() + getPoints(3),  // +3mm top
                getPoints(80),                        // 80mm width
                getPoints(40)                         // 40mm height
  );
}

const QPageLayout APrintTools::pageLayout() const {
  QPageLayout _layout;
  _layout.setOrientation(QPageLayout::Portrait);
  _layout.setPageSize(pageSize);
  _layout.setMinimumMargins(QMargins(0, 0, 0, 0));
  _layout.setMargins(QMargins(0, 0, 0, 0));
  _layout.setUnits(QPageLayout::Millimeter);
  _layout.setMode(QPageLayout::FullPageMode);
  return _layout;
}

const QTextOption APrintTools::textOption(Qt::Alignment align,
                                          QTextOption::WrapMode mode) {
  QTextOption _opts(align);
  _opts.setWrapMode(mode);
  return _opts;
}

const QPen APrintTools::linePen(const QColor &color) const {
  QPen _pen(color);
  _pen.setStyle(Qt::SolidLine);
  _pen.setCapStyle(Qt::FlatCap);
  _pen.setJoinStyle(Qt::RoundJoin);
  _pen.setWidthF(0.6);
  _pen.setCosmetic(true);
  return _pen;
}

const QPen APrintTools::fontPen(const QColor &color) const {
  QPen _pen(color);
  _pen.setCosmetic(true);
  return _pen;
}

qreal APrintTools::fontHeight(const QFont &font) const {
  const QFontMetrics _metrics(font);
  return _metrics.height();
}

const QBrush APrintTools::borderBrush(const QColor &color) const {
  return QBrush(color, Qt::SolidPattern);
}

} // namespace AntiquaCRM
