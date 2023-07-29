// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprinttools.h"

#include <QRegularExpression>

namespace AntiquaCRM {

APrintTools::APrintTools(QPageSize::PageSizeId id) : pageSize{id} {}

qreal APrintTools::getPoints(int millimeter) const {
  return qRound(millimeter * points);
}

const QPen APrintTools::linePen(const QColor &color) const {
  QPen _pen(color, Qt::SolidLine);
  _pen.setCapStyle(Qt::FlatCap);
  _pen.setWidthF(0.6);
  _pen.setCosmetic(true);
  return _pen;
}

const QPen APrintTools::fontPen(const QColor &color) const {
  QPen _pen(color, Qt::SolidLine);
  _pen.setCosmetic(true);
  return _pen;
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
  return QRectF(getPoints(left), // 25mm left
                getPoints(45),   // 45mm top
                getPoints(80),   // 80mm width
                getPoints(45)    // 45mm height
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

const QBrush APrintTools::borderBrush(const QColor &color) const {
  return QBrush(color, Qt::SolidPattern);
}

} // namespace AntiquaCRM
