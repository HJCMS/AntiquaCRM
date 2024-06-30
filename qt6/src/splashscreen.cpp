// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "splashscreen.h"

#include <QtCore>
#include <QtGui>

SplashScreen::SplashScreen(QApplication* parent)
    : QSplashScreen{parent->primaryScreen(), background()}, // Splash
      title{ANTIQUACRM_WINDOW_TITLE} {
  QFont f(parent->font().family(), 12);
  setFont(f);
  setMessage(tr("Application initialization ..."));
}

const QFont SplashScreen::titleFont() const {
  QString _fontName("Steve");
  QStringList _flist = QFontDatabase::families(QFontDatabase::Latin);
  if (!_flist.contains(_fontName)) {
    int ret = -1;
#ifdef Q_OS_WIN
    QDir _d = QDir::current();
    _d.cd("data" + QString(QDir::separator()) + "fonts");
    QFileInfo _ttf(_d.path(), "steve.ttf");
    if (_ttf.isReadable()) {
      QFile _fp(_ttf.filePath());
      if (_fp.open(QFile::ReadOnly)) {
        QByteArray _array = _fp.readAll();
        if (!_array.isNull()) {
          ret = QFontDatabase::addApplicationFontFromData(_array);
        }
        _fp.close();
      }
    }
    _d.cdUp();
#endif
    if (ret == -1) {
      _fontName = QString("Luxi Mono");
#ifdef ANTIQUA_DEVELOPMENT
      qWarning("Splash: font file not found fallback to „%s“.", qPrintable(_fontName));
#endif
    }
  }
  _flist.clear();

  QFont _font;
  _font.fromString(_fontName);
  _font.setItalic(true);
  _font.setPointSize(28);
  return _font;
}

const QPixmap SplashScreen::background() const {
  QPixmap pixmap(QString("://icons/antiquacrm-splash.png"));
  return pixmap;
}

void SplashScreen::drawContents(QPainter* painter) {
  // Nachrichten Hintergrund
  QPoint start(0, rect().height() - (font().pointSize() * 3));
  QPoint final(width(), height());
  QLinearGradient gradient(start, final);
  gradient.setSpread(QGradient::ReflectSpread);
  gradient.setColorAt(0, Qt::white);
  gradient.setColorAt(1, Qt::transparent);
  painter->fillRect(QRect(start, final), gradient);
  // Standard Nachrichtentext
  QSplashScreen::drawContents(painter);
  // Programmtitel
  painter->setFont(titleFont());
  QRect r = painter->boundingRect(rect(), // Splash Rect
                                  (Qt::AlignLeft | Qt::AlignTop), title);
  QRect tbRect(11, 11, r.width(), r.height());
  painter->setPen(Qt::lightGray);
  painter->drawText(tbRect, title);
  QRect tfRect(10, 10, r.width(), r.height());
  painter->setPen(Qt::black);
  painter->drawText(tfRect, title);
  painter->end();
}

void SplashScreen::errorMessage(const QString& message) {
  showMessage(message, Qt::AlignBottom, Qt::darkRed);
}

void SplashScreen::setMessage(const QString& message) {
  showMessage(message, Qt::AlignBottom, Qt::black);
}
