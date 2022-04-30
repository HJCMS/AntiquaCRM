// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagetoolbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QtWidgets/QHBoxLayout>

ImageToolBar::ImageToolBar(QWidget *parent) : QWidget{parent} {
  setObjectName("ImageActionToolBar");
  setContentsMargins(0, 1, 0, 1);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 1, 0, 1);
  layout->addStretch(1);

  m_openbtn = new QPushButton(myIcon("image"), tr("Import"), this);
  m_openbtn->setToolTip(
      tr("Open the Imaging Dialog for Import and Edit Pictures."));
  layout->addWidget(m_openbtn);

  m_delbtn = new QPushButton(myIcon("edit_remove"), tr("Remove"), this);
  m_delbtn->setToolTip(tr("Remove image from Database."));
  m_delbtn->setEnabled(false);
  layout->addWidget(m_delbtn);

  setLayout(layout);

  connect(m_openbtn, SIGNAL(clicked()), this, SLOT(checkOpen()));
  connect(m_delbtn, SIGNAL(clicked()), this, SLOT(checkRemove()));
}

void ImageToolBar::checkOpen() { emit s_openImage(); }

void ImageToolBar::checkRemove() {
  if (aId > 0)
    emit s_deleteImage(aId);
}

void ImageToolBar::setArticleId(int id) {
  aId = id;
  m_delbtn->setEnabled((aId > 1));
}

void ImageToolBar::setActive(bool b) { m_openbtn->setEnabled(b); }
