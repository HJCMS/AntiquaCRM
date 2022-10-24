// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <AntiquaCRM>

Inventory::Inventory(QWidget *parent) : QStackedWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  setWindowIcon(getTabIcon());
  addShortCutsAndSignals();
}

void Inventory::addShortCutsAndSignals() {
  Qt::KeyboardModifiers km(Qt::ControlModifier + Qt::ShiftModifier);

  m_focusSearch = new QShortcut(tr("Ctrl+Shift+S", "Search"), this);
  m_focusSearch->setKey(km + Qt::Key_S);
  connect(m_focusSearch, SIGNAL(activated()), this,
          SIGNAL(sendSetSearchFocus()));

  m_focusFilter = new QShortcut(tr("Ctrl+Shift+F", "Filter"), this);
  m_focusFilter->setKey(km + Qt::Key_F);
  connect(m_focusFilter, SIGNAL(activated()), this,
          SIGNAL(sendSetSearchFilter()));

  m_createEntry = new QShortcut(tr("Ctrl+Shift+N", "New"), this);
  m_createEntry->setKey(km + Qt::Key_N);
  connect(m_createEntry, SIGNAL(activated()), this, SLOT(createNewEntry()));
}

void Inventory::setClosable(bool b) {
  closable = b;
  emit sendClosableChanged();
}

void Inventory::changeEvent(QEvent *event) {
  if (event->type() == QEvent::EnabledChange)
    emit sendEnabledStatus(isEnabled());

  QStackedWidget::changeEvent(event);
}

void Inventory::copyToClipboard(const QString &data) {
  QString buf = data.trimmed();
  if (buf.isEmpty())
    return;

  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void Inventory::sendStatusMessage(const QString &message) {
  AntiquaCRM::AStatusMessanger messanger(this);
  messanger.setObjectName("tab_status_message");
  QJsonObject obj;
  obj.insert("window_status_message",message);
  messanger.pushMessage(obj);
  messanger.close();
}

bool Inventory::isClosable() { return closable; }

bool Inventory::isModified() { return isWindowModified(); }

const QIcon Inventory::getTabIcon(const QString &name) {
  return QIcon(QString(":icons/" + name + ".png"));
}
