// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"

#include <AntiquaCRM>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

Inventory::Inventory(const char *index, QWidget *parent)
    : QStackedWidget{parent}, tabIndex{index} {
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
  AntiquaCRM::ATxSocket *m_sock = new AntiquaCRM::ATxSocket(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushStatusBarMessage(message))
    m_sock->close();
}

void Inventory::sendSocketOperation(const QJsonObject &obj) {
  AntiquaCRM::ATxSocket *m_sock = new AntiquaCRM::ATxSocket(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(obj))
    m_sock->close();
}

void Inventory::openWarningPopUp(const QString &title, const QString &message) {
  QString info("<p>" + title + "</p><p>" + message + "</p>");
  QMessageBox::warning(this, tr("Warning"), info);
}

bool Inventory::isClosable() { return closable; }

bool Inventory::isModified() { return isWindowModified(); }

const QString Inventory::tabIndexId() const { return tabIndex; }

const QIcon Inventory::getTabIcon(const QString &name) {
  return QIcon(QString("://icons/" + name + ".png"));
}
