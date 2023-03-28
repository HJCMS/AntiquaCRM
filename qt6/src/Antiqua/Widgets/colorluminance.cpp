// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "colorluminance.h"

//#include <QDebug>
#include <QRegularExpression>
#include <QStringList>

namespace AntiquaCRM {

ColorLuminance::ColorLuminance(QApplication *parent)
    : QObject{parent}, p_palette{parent->palette()} {
  Q_CHECK_PTR(parent);
}

double ColorLuminance::luminance(const QColor &color) {
  double _r, _g, _b;
  double _rsrgb = (color.red() / 255.0);
  if (_rsrgb <= 0.03928) {
    _r = (_rsrgb / 12.92);
  } else {
    _r = qPow(((_rsrgb + 0.055) / 1.055), 2.4);
  }

  double _gsrgb = (color.green() / 255.0);
  if (_gsrgb <= 0.03928) {
    _g = (_gsrgb / 12.92);
  } else {
    _g = qPow(((_gsrgb + 0.055) / 1.055), 2.4);
  }

  double _bsrgb = (color.blue() / 255.0);
  if (_bsrgb <= 0.03928) {
    _b = (_bsrgb / 12.92);
  } else {
    _b = qPow(((_bsrgb + 0.055) / 1.055), 2.4);
  }

  return ((0.2126 * _r) + (0.7152 * _g) + (0.0722 * _b));
}

bool ColorLuminance::check(const QColor &fg, const QColor &bg) const {
  double _crf = (luminance(fg) + 0.05);
  double _crb = (luminance(bg) + 0.05);
  double _lum = (_crf > _crb) ? qRound(_crf / _crb) : qRound(_crb / _crf);
  return (_lum > 11);
}

bool ColorLuminance::checkForeground(const QColor &fgColor) {
  QColor _fc = fgColor.toRgb();
  QColor _bc = p_palette.window().color().toRgb();
  return check(_fc, _bc);
}

bool ColorLuminance::checkBackground(const QColor &bgColor) {
  QColor _fc = p_palette.text().color().toRgb();
  QColor _bc = bgColor.toRgb();
  return check(_fc, _bc);
}

} // namespace AntiquaCRM
