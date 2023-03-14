// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasplitter.h"

#include <QDebug>

AntiquaSplitterHandle::AntiquaSplitterHandle(QSplitter *parent)
    : QSplitterHandle{Qt::Horizontal, parent} {}

void AntiquaSplitterHandle::mouseReleaseEvent(QMouseEvent *e) {
  emit sendSaveState();
  QSplitterHandle::mouseReleaseEvent(e);
}

AntiquaSplitter::AntiquaSplitter(QWidget *parent)
    : QSplitter{Qt::Horizontal, parent} {
  setObjectName("antiqua_hsplitter");
  setChildrenCollapsible(false);
  setContentsMargins(0, 0, 0, 0);
  QStringList css("::handle {margin:4px;padding:0px;}");
  css.append("::handle:pressed {background-color:palette(highlight);}");
  setStyleSheet(css.join(" "));
  m_cfg = new AntiquaCRM::ASettings(this);

  if (parent->objectName().isEmpty()) {
    qWarning("AntiquaSplitter need a objectName from parent!");
    name = objectName();
  } else {
    name = parent->objectName().toLower().replace(" ", "").trimmed();
  }
}

const QString AntiquaSplitter::configPath() {
  QString p("splitter/");
  p.append("antiqua_sp_handle_");
  p.append(name);
  return p;
}

AntiquaSplitterHandle *AntiquaSplitter::createHandle() {
  AntiquaSplitterHandle *sph = new AntiquaSplitterHandle(this);
  sph->setObjectName(name);
  connect(sph, SIGNAL(sendSaveState()), SLOT(setSaveState()));
  return sph;
}

void AntiquaSplitter::showEvent(QShowEvent *event) {
  if (event->type() == QEvent::Show) {
    QByteArray state = m_cfg->value(configPath()).toByteArray();
    if (!state.isNull())
      restoreState(state);
  }
  QSplitter::showEvent(event);
}

void AntiquaSplitter::setSaveState() {
  m_cfg->setValue(configPath(), saveState());
}

void AntiquaSplitter::addLeft(QWidget *widget) { insertWidget(0, widget); }

void AntiquaSplitter::addRight(QWidget *widget) { insertWidget(1, widget); }

AntiquaSplitter::~AntiquaSplitter() {
  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}
