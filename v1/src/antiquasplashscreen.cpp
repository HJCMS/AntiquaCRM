// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasplashscreen.h"

#include <AGlobal>
#include <QBrush>
#include <QColor>
#include <QDataStream>
#include <QFile>
#include <QFont>
#include <QGradient>
#include <QIODevice>
#include <QPainter>
#include <QFontDatabase>
#include <QPoint>

static const QPixmap splash() {
  QPixmap pixmap(QString("://icons/splash.png"));
  return pixmap;
}

static const QFont headerFont() {
  QFile fp(QString("://data/steve.tff"));
  if (fp.open(QFile::ReadOnly)) {
    QByteArray fontArray = fp.readAll();
    QFontDatabase::addApplicationFontFromData(fontArray);
    fp.close();
  }

  QFont font;
  font.fromString("Steve");
  font.setItalic(true);
  font.setPointSize(30);
  return font;
}

AntiquaSplashScreen::AntiquaSplashScreen(QMainWindow *parent)
    : QSplashScreen{parent->screen(), splash()}, // Splash
      title{ANTIQUACRM_WINDOW_TITLE} {
  QFont f(font().family(), 12);
  setFont(f);

  titleFont = headerFont();
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
  painter->setFont(titleFont);
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
