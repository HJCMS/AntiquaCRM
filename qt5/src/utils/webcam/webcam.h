// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_WEBCAM_H
#define ANTIQUA_WEBCAM_H

#include <QDialog>
#include <QWidget>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

class WebCam final : public QDialog {
  Q_OBJECT

private:
  QMediaPlayer *m_player;
  QVideoWidget *m_videoWidget;

public:
  explicit WebCam(QWidget *parent = nullptr);
  static bool checkWebcam();
  int exec() override;
};

#endif
