// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "splitter.h"

#include <QDebug>

namespace AntiquaCRM {

SplitterHandle::SplitterHandle(QSplitter *parent)
    : QSplitterHandle{Qt::Horizontal, parent} {}

void SplitterHandle::mouseReleaseEvent(QMouseEvent *e) {
  emit sendSaveState();
  QSplitterHandle::mouseReleaseEvent(e);
}

Splitter::Splitter(QWidget *parent) : QSplitter{Qt::Horizontal, parent} {
  setObjectName("antiqua_hsplitter");
  setChildrenCollapsible(false);
  setContentsMargins(0, 0, 0, 0);
  QStringList css("::handle {margin:4px;padding:0px;}");
  css.append("::handle:pressed {background-color:palette(highlight);}");
  setStyleSheet(css.join(" "));
  m_cfg = new AntiquaCRM::ASettings(this);

  if (parent->objectName().isEmpty()) {
    qWarning("Splitter need a objectName from parent!");
    name = objectName();
  } else {
    name = parent->objectName().toLower().replace(" ", "").trimmed();
  }
}

const QString Splitter::configPath() {
  QString p("splitter/");
  p.append("antiqua_sp_handle_");
  p.append(name);
  return p;
}

SplitterHandle *Splitter::createHandle() {
  SplitterHandle *sph = new SplitterHandle(this);
  sph->setObjectName(name);
  connect(sph, SIGNAL(sendSaveState()), SLOT(setSaveState()));
  return sph;
}

void Splitter::showEvent(QShowEvent *event) {
  if (event->type() == QEvent::Show) {
    QByteArray state = m_cfg->value(configPath()).toByteArray();
    if (!state.isNull())
      restoreState(state);
  }
  QSplitter::showEvent(event);
}

void Splitter::setSaveState() { m_cfg->setValue(configPath(), saveState()); }

void Splitter::addLeft(QWidget *widget) { insertWidget(0, widget); }

void Splitter::addRight(QWidget *widget) { insertWidget(1, widget); }

Splitter::~Splitter() {
  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

} // namespace AntiquaCRM
