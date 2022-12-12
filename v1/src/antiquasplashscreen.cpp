// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasplashscreen.h"

#include <AGlobal>
#include <QtCore>
#include <QtGui>

AntiquaSplashScreen::AntiquaSplashScreen(QMainWindow *parent)
    : QSplashScreen{parent->screen(), background()}, // Splash
      title{ANTIQUACRM_WINDOW_TITLE} {
  QFont f(font().family(), 12);
  setFont(f);
}

const QFont AntiquaSplashScreen::titleFont() const {
  int ret = -1;
  QString fontName("Steve");
  QDir p = QDir::current();
  p.cd("data" + QString(QDir::separator()) + "fonts");
  QFileInfo ttf(p.path(), "steve.ttf");
  if (ttf.isReadable()) {
    QFile fp(ttf.filePath());
    if (fp.open(QFile::ReadOnly)) {
      QByteArray fontArray = fp.readAll();
      ret = QFontDatabase::addApplicationFontFromData(fontArray);
      fp.close();
    }
  }
  p.cdUp();

  if (ret == -1) {
    qWarning("Splash font file not add.");
    fontName = QString("Luxi Mono");
  }

  QFont font;
  font.fromString(fontName);
  font.setItalic(true);
  font.setPointSize(30);
  return font;
}

const QPixmap AntiquaSplashScreen::background() const {
  QPixmap pixmap(QString("://icons/splash.png"));
  return pixmap;
}

void AntiquaSplashScreen::drawContents(QPainter *painter) {
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

void AntiquaSplashScreen::setMessage(const QString &message) {
  showMessage(message, Qt::AlignBottom, Qt::black);
}
