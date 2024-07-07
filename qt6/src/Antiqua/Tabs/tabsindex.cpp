// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsindex.h"
#include "tabseditor.h"

#include <AntiquaInput>
#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QMessageBox>

namespace AntiquaCRM
{

TabsIndex::TabsIndex(const char* index, QWidget* parent) : QStackedWidget{parent}, tabIndex{index} {
  setContentsMargins(0, 0, 0, 0);
  setWindowIcon(antiquaIcon("antiquacrm"));
  addShortCutsAndSignals();
}

void TabsIndex::addShortCutsAndSignals() {
  Qt::KeyboardModifiers _km(Qt::ControlModifier | Qt::ShiftModifier);

  m_focusSearch = new QShortcut(tr("Ctrl+Shift+S", "Search"), this);
  m_focusSearch->setKey(_km | Qt::Key_S);
  connect(m_focusSearch, SIGNAL(activated()), SLOT(prepareShortCutSearchFocus()));

  m_focusFilter = new QShortcut(tr("Ctrl+Shift+F", "Filter"), this);
  m_focusFilter->setKey(_km | Qt::Key_F);
  connect(m_focusFilter, SIGNAL(activated()), SLOT(prepareShortCutSearchFilter()));

  m_createEntry = new QShortcut(tr("Ctrl+Shift+N", "New"), this);
  m_createEntry->setKey(_km | Qt::Key_N);
  connect(m_createEntry, SIGNAL(activated()), SLOT(prepareShortCutNewEntry()));
}

void TabsIndex::prepareShortCutSearchFocus() {
  if (currentPage() == TabsIndex::ViewPage::MainView)
    emit sendSetSearchFocus();
}

void TabsIndex::prepareShortCutSearchFilter() {
  if (currentPage() == TabsIndex::ViewPage::MainView)
    emit sendSetSearchFilter();
}

void TabsIndex::prepareShortCutNewEntry() {
  if (currentPage() == TabsIndex::ViewPage::MainView)
    createNewEntry();
}

void TabsIndex::setClosable(bool b) {
  closable = b;
  if (b)
    emit sendClosableChanged();
}

bool TabsIndex::eventFilter(QObject* obj, QEvent* event) {
  if (event->type() == QEvent::ModifiedChange) {
    AntiquaCRM::TabsEditor* m_te = qobject_cast<AntiquaCRM::TabsEditor*>(obj);
    if (m_te != nullptr) {
      bool _status = m_te->isWindowModified();
      setWindowModified(_status);
      emit sendModifiedStatus(_status);
      return true;
    }
  }
  return QStackedWidget::eventFilter(obj, event);
}

void TabsIndex::copyToClipboard(const QString& data) {
  QString _buf = data.trimmed();
  if (_buf.isEmpty())
    return;

  QApplication::clipboard()->setText(_buf, QClipboard::Clipboard);
}

void TabsIndex::sendStatusMessage(const QString& message) {
  AntiquaCRM::ATransmitter* _sock = new AntiquaCRM::ATransmitter(this);
  connect(_sock, SIGNAL(disconnected()), _sock, SLOT(deleteLater()));
  if (_sock->pushStatusBarMessage(message))
    _sock->close();
}

void TabsIndex::sendSocketOperation(const QJsonObject& obj) {
  AntiquaCRM::ATransmitter* _sock = new AntiquaCRM::ATransmitter(this);
  connect(_sock, SIGNAL(disconnected()), _sock, SLOT(deleteLater()));
  if (_sock->pushOperation(obj))
    _sock->close();
}

void TabsIndex::openWarningPopUp(const QString& title, const QString& message) {
  QString _info("<p>" + title + "</p><p>" + message + "</p>");
  QMessageBox::warning(this, tr("Warning"), _info);
}

void TabsIndex::openWarningPopUpPageIndex(const QString& title) {
  QString _t = QString("„%1“").arg(title.trimmed().remove("[*]").trimmed());
  QString _m(tr("Can't open %1 tab.").arg(_t));
  _m.append("<p>");
  _m.append(tr("Because %1 tab is in edit mode.").arg(_t));
  _m.append("</p><p>");
  _m.append(tr("You have first to check, complete the %1 edit mode.").arg(_t));
  _m.append("</p><p>");
  _m.append(tr("Otherwise you can lose changes."));
  _m.append("</p>");
  QMessageBox::information(this, _t, _m);
}

const QString TabsIndex::tabIndexId() const {
  return tabIndex;
}

bool TabsIndex::isClosable() {
  return closable;
}

AntiquaCRM::TabsIndex::ViewPage TabsIndex::currentPage() {
  switch (currentIndex()) {
    case 0:
      return AntiquaCRM::TabsIndex::MainView;

    case 1:
      return AntiquaCRM::TabsIndex::EditorView;

    case 2:
      return AntiquaCRM::TabsIndex::CustomView;

    default:
      return AntiquaCRM::TabsIndex::MainView;
  }
}

} // namespace AntiquaCRM
