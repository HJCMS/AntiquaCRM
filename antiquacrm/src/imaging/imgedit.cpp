// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imgedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

ImgEdit::ImgEdit(QWidget *parent) : QWidget{parent} {
  setObjectName("ImgEditor");

  QVBoxLayout *m_vLayout = new QVBoxLayout(this);

  QLabel *lb_found = new QLabel(this);
  lb_found->setText(tr("Found Images"));
  m_vLayout->addWidget(lb_found);

  m_listFiles = new QListWidget(this);
  m_vLayout->addWidget(m_listFiles);

  m_vLayout->addStretch();

  QLabel *lb_gamma = new QLabel(this);
  lb_gamma->setText(tr("Gamma"));
  m_vLayout->addWidget(lb_gamma);

  m_gammaSlider = new QSlider(Qt::Horizontal, this);
  m_gammaSlider->setMaximum(10);
  m_gammaSlider->setMinimum(-10);
  m_gammaSlider->setSliderPosition(0);
  m_vLayout->addWidget(m_gammaSlider);

  m_imageInfo = new QLabel(this);
  m_vLayout->addWidget(m_imageInfo);

  setLayout(m_vLayout);
}

void ImgEdit::addFileList(const QStringList &l) {
  foreach (QString f, l) {
    m_listFiles->addItem(f);
  }
}
