// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_SPLASHSCREEN_H
#define ANTIQUA_UI_SPLASHSCREEN_H

#include <AGlobal>
#include <QFont>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QSplashScreen>

class SplashScreen final : public QSplashScreen {
  Q_OBJECT

private:
  const QString title;
  const QFont titleFont() const;
  inline const QPixmap background() const;
  void drawContents(QPainter *painter) override;

public Q_SLOTS:
  void errorMessage(const QString &);
  void setMessage(const QString &);

public:
  explicit SplashScreen(QMainWindow *parent);
};

#endif // ANTIQUA_UI_SPLASHSCREEN_H
