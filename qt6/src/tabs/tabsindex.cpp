// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsindex.h"

#include <AntiquaCRM>
#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QMessageBox>

TabsIndex::TabsIndex(const char *index, QWidget *parent)
    : QStackedWidget{parent}, tabIndex{index} {
  setContentsMargins(0, 0, 0, 0);
  setWindowIcon(getTabIcon());
  addShortCutsAndSignals();
}

void TabsIndex::addShortCutsAndSignals() {
  Qt::KeyboardModifiers _km(Qt::ControlModifier | Qt::ShiftModifier);

  m_focusSearch = new QShortcut(tr("Ctrl+Shift+S", "Search"), this);
  m_focusSearch->setKey(_km | Qt::Key_S);
  connect(m_focusSearch, SIGNAL(activated()), this,
          SIGNAL(sendSetSearchFocus()));

  m_focusFilter = new QShortcut(tr("Ctrl+Shift+F", "Filter"), this);
  m_focusFilter->setKey(_km | Qt::Key_F);
  connect(m_focusFilter, SIGNAL(activated()), this,
          SIGNAL(sendSetSearchFilter()));

  m_createEntry = new QShortcut(tr("Ctrl+Shift+N", "New"), this);
  m_createEntry->setKey(_km | Qt::Key_N);
  connect(m_createEntry, SIGNAL(activated()), this, SLOT(createNewEntry()));
}

bool TabsIndex::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ModifiedChange) {
    // InventoryEditor *edit = qobject_cast<InventoryEditor *>(obj);
    // if (edit != nullptr) {
    //  bool status = edit->isWindowModified();
    //  setWindowModified(status);
    //  emit sendUnsafedChanges(status);
    //  return true;
    // }
  }
  return QStackedWidget::eventFilter(obj, event);
}

void TabsIndex::setClosable(bool b) {
  closable = b;
  emit sendClosableChanged();
}

void TabsIndex::changeEvent(QEvent *event) {
  if (event->type() == QEvent::EnabledChange)
    emit sendEnabledStatus(isEnabled());

  QStackedWidget::changeEvent(event);
}

void TabsIndex::copyToClipboard(const QString &data) {
  QString buf = data.trimmed();
  if (buf.isEmpty())
    return;

  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void TabsIndex::sendStatusMessage(const QString &message) {
  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushStatusBarMessage(message))
    m_sock->close();
}

void TabsIndex::sendSocketOperation(const QJsonObject &obj) {
  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(obj))
    m_sock->close();
}

void TabsIndex::openWarningPopUp(const QString &title, const QString &message) {
  QString info("<p>" + title + "</p><p>" + message + "</p>");
  QMessageBox::warning(this, tr("Warning"), info);
}

const QString TabsIndex::tabIndexId() const { return tabIndex; }

const QIcon TabsIndex::getTabIcon(const QString &name) {
  return QIcon("://icons/" + name + ".png");
}

bool TabsIndex::isClosable() { return closable; }
