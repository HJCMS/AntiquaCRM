// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagetoolbar.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>

ImageToolBar::ImageToolBar(QWidget *parent) : QWidget{parent} {
  setObjectName("ImageActionToolBar");
  setContentsMargins(0, 1, 0, 1);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 1, 0, 1);
  layout->addStretch(1);

  QIcon img_icon(":icons/view_image.png");
  QIcon del_icon(":icons/action_remove.png");

  m_openbtn = new QPushButton(img_icon, tr("Image Dialog"), this);
  m_openbtn->setToolTip(tr("Dialog to Import/Edit Images."));
  layout->addWidget(m_openbtn);

  m_webcambtn = new QPushButton(img_icon, tr("Webcam Dialog"), this);
  m_webcambtn->setToolTip(tr("Import Image from a Webcam."));
  m_webcambtn->setEnabled(false);
  layout->addWidget(m_webcambtn);

  m_delbtn = new QPushButton(del_icon, tr("Remove Image"), this);
  m_delbtn->setToolTip(tr("Remove image from Database."));
  m_delbtn->setEnabled(false);
  layout->addWidget(m_delbtn);

  setLayout(layout);

  connect(m_openbtn, SIGNAL(clicked()), SLOT(checkOpen()));
  connect(m_delbtn, SIGNAL(clicked()), SLOT(checkRemove()));
}

void ImageToolBar::checkOpen() { emit sendOpenImage(); }

void ImageToolBar::checkRemove() {
  if (articleId > 0)
    emit sendDeleteImage(articleId);
}

void ImageToolBar::setArticleId(qint64 id) { articleId = id; }

void ImageToolBar::setActive(bool b) { m_openbtn->setEnabled(b); }

void ImageToolBar::enableActions(bool b) {
  m_delbtn->setEnabled(b);
  // m_webcambtn->setEnabled(b);
}

void ImageToolBar::restoreState() {
  articleId = 0;
  m_openbtn->setEnabled(false);
  m_delbtn->setEnabled(false);
  m_webcambtn->setEnabled(false);
}
