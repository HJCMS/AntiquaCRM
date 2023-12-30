// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsstatusprogress.h"

#include <QHBoxLayout>
#include <QStyleOptionProgressBar>
#include <QStylePainter>

namespace AntiquaCRM {

TabsProgressAnimation::TabsProgressAnimation(QProgressBar *parent)
    : QPropertyAnimation{parent} {
  setTargetObject(parent);
  setPropertyName("value");
  setDuration(50);
  setStartValue(10);
  setEndValue(0);
}

TabsStatusProgress::TabsStatusProgress(QWidget *parent) : QProgressBar{parent} {
  setObjectName("tabs_progress_bar");
  setContentsMargins(0, 0, 0, 0);
  setTextVisible(false);
  setRange(0, 10);
  setStyleSheet("QProgressBar {background:transparent;border:none;}");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(contentsMargins());
  m_info = new QLabel(this);
  m_info->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  m_info->setIndent(4);
  layout->addWidget(m_info);
  setLayout(layout);
}

TabsStatusProgress::~TabsStatusProgress() { m_info->clear(); }

void TabsStatusProgress::showMessage(const QString &message, int timeout) {
  Q_UNUSED(timeout);
  m_info->setText(message);
}

void TabsStatusProgress::reset() { setValue(0); }

void TabsStatusProgress::start(const QString &message) {
  m_info->setText(message);
  setValue(100);
  (new TabsProgressAnimation(this))->start();
}

} // namespace AntiquaCRM
