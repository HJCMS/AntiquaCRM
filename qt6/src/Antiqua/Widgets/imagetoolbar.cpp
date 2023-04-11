// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagetoolbar.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>

namespace AntiquaCRM {

ImageToolBar::ImageToolBar(QWidget *parent) : QToolBar{parent} {
  setContentsMargins(0, 1, 0, 1);

  QIcon img_icon = AntiquaApplIcon("view-image");
  QIcon del_icon = AntiquaApplIcon("action-remove");

  m_btnOpen = new QPushButton(img_icon, tr("Image Dialog"), this);
  m_btnOpen->setToolTip(tr("Dialog to Edit/Import Images."));
  connect(m_btnOpen, SIGNAL(clicked()), SIGNAL(sendOpenImage()));
  addWidget(m_btnOpen);

  m_btnRemove = new QPushButton(del_icon, tr("Remove Image"), this);
  m_btnRemove->setToolTip(tr("Remove image from Database."));
  m_btnRemove->setEnabled(false);
  connect(m_btnRemove, SIGNAL(clicked()), SLOT(beforeRemove()));
  addWidget(m_btnRemove);
}

void ImageToolBar::beforeRemove() {
  if (articleId > 0)
    emit sendDeleteImage(articleId);
}

void ImageToolBar::setArticleId(qint64 id) {
  articleId = id;
  m_btnRemove->setEnabled(articleId > 0);
}

void ImageToolBar::setRestoreState() {
  articleId = -1;
  m_btnRemove->setEnabled(false);
}

qint64 ImageToolBar::getArticleId() { return articleId; }

} // namespace AntiquaCRM
