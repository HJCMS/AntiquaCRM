// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SPLASHSCREEN_H
#define ANTIQUACRM_SPLASHSCREEN_H

#include <AGlobal>
#include <QFont>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QSplashScreen>

class AntiquaSplashScreen : public QSplashScreen {
  Q_OBJECT

private:
  const QString title;
  const QFont titleFont() const;
  const QPixmap background() const;
  void drawContents(QPainter *painter) override;

public Q_SLOTS:
  Q_INVOKABLE void setMessage(const QString &);

public:
  explicit AntiquaSplashScreen(QMainWindow *parent = nullptr);
};

#endif // ANTIQUACRM_SPLASHSCREEN_H
