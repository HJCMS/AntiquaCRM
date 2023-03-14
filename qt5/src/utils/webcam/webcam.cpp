// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "webcam.h"

#include <QCameraInfo>
#include <QDebug>
#include <QLayout>

WebCam::WebCam(QWidget *parent) : QDialog{parent} {
  setObjectName("webcam_dialog");
  setWindowTitle(tr("Webcam"));
  setMinimumSize(QSize(450, 250));
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_videoWidget = new QVideoWidget(this);
  layout->addWidget(m_videoWidget);

  setLayout(layout);
}

bool WebCam::checkWebcam() {
  foreach (const QCameraInfo &info, QCameraInfo::availableCameras()) {
    if (!info.isNull())
      return true;
  }
  return false;
}

int WebCam::exec() {
  const QList<QCameraInfo> list = QCameraInfo::availableCameras();
  foreach (const QCameraInfo info, list) {
    if (!info.isNull()) {
      // Player
      m_player = new QMediaPlayer;
      m_player->setVideoOutput(m_videoWidget);

      QCamera *m_webcam = new QCamera(info, m_player);
      if (m_webcam != nullptr) {
        m_webcam->setCaptureMode(QCamera::CaptureVideo);

        m_videoWidget->show();
        m_player->play();
        break;
      }
    }
  }
  return QDialog::exec();
}
