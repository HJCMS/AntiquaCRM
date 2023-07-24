// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buyerinfo.h"

#include <QLabel>
#include <QLayout>

BuyerInfo::BuyerInfo(QWidget *parent) : QWidget{parent} {
  setContentsMargins(2, 0, 0, 0);

  const QString _label_css("QLabel {margin-left:2px; font-style:italic;}");
  const QString _textedit_css("QTextEdit {background: transparent;border-top: "
                              "1px solid palette(text);}");

  QHBoxLayout *layout = new QHBoxLayout(this);
  // Left
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->setContentsMargins(0, 0, 0, 0);
  QLabel *ainfo = new QLabel(this);
  ainfo->setText(tr("Address"));
  ainfo->setStyleSheet(_label_css);
  ainfo->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
  leftLayout->addWidget(ainfo);
  m_address = new QTextEdit(this);
  m_address->setReadOnly(true);
  m_address->setStyleSheet(_textedit_css);
  leftLayout->addWidget(m_address);
  leftLayout->addStretch(1);
  layout->addLayout(leftLayout);
  // Right
  QVBoxLayout *rightLayout = new QVBoxLayout();
  rightLayout->setContentsMargins(0, 0, 0, 0);
  QLabel *binfo = new QLabel(this);
  binfo->setText(tr("Comment"));
  binfo->setStyleSheet(_label_css);
  binfo->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
  rightLayout->addWidget(binfo);
  m_comment = new QTextEdit(this);
  m_comment->setReadOnly(true);
  m_comment->setStyleSheet(_textedit_css);
  rightLayout->addWidget(m_comment);
  rightLayout->addStretch(1);
  layout->addLayout(rightLayout);
  setLayout(layout);
}

void BuyerInfo::setAddress(const QString &str) { m_address->setPlainText(str); }

void BuyerInfo::setComment(const QString &str) { m_comment->setPlainText(str); }
