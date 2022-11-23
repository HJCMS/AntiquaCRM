// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerbuyerinfo.h"

#include <QLabel>
#include <QLayout>
#include <QSizePolicy>

ProviderTextBlock::ProviderTextBlock(const QString &title, QWidget *parent)
    : QFrame{parent} {
  setContentsMargins(2, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  m_info->setText(title);
  m_info->setStyleSheet("QLabel {margin-left:2px; font-style:italic;}");
  m_info->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
  layout->addWidget(m_info);

  QStringList css("background: transparent;");
  css << "border-top: 1px solid palette(text);";

  m_edit = new QTextEdit(this);
  m_edit->setReadOnly(true);
  m_edit->setStyleSheet("QTextEdit {" + css.join(" ") + "}");
  layout->addWidget(m_edit);

  layout->addStretch(1);
  setLayout(layout);
}

void ProviderTextBlock::setContent(const QString &str) {
  m_edit->setPlainText(str);
}

ProviderBuyerInfo::ProviderBuyerInfo(QWidget *parent) : QSplitter{parent} {
  setOrientation(Qt::Horizontal);
  setHandleWidth(0);

  m_invoice = new ProviderTextBlock(tr("Address"), this);
  m_invoice->setToolTip(tr("Buyer invoice address"));
  insertWidget(0, m_invoice);

  m_comment = new ProviderTextBlock(tr("Buyer Comments"), this);
  m_comment->setToolTip(tr("Some Buyer Notification Messages."));
  insertWidget(1, m_comment);

  setStretchFactor(0, 30);
  setCollapsible(0, false);
  setStretchFactor(1, 70);
  setCollapsible(1, false);
}

void ProviderBuyerInfo::setAddress(const QString &str) {
  if (str.isEmpty())
    return;

  m_invoice->setContent(str);
}

void ProviderBuyerInfo::setBuyerComment(const QString &str) {
  if (str.isEmpty())
    return;

  m_comment->setContent(str);
}
