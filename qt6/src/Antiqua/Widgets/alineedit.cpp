// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"
#include "antiquaicon.h"
#include "private/lineinputvalidator.h"

#include <AntiquaCRM>
#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QCompleter>
#include <QIcon>
#include <QPropertyAnimation>
#include <QRegularExpressionValidator>
#include <QStatusTipEvent>
#include <QTimer>
#include <QValidator>

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent} {
  setObjectName("ALineEdit");
  setClearButtonEnabled(true);
  setFocusPolicy(Qt::StrongFocus);
  ac_completer = addAction(AntiquaCRM::antiquaIcon("view-list-details"),
                           QLineEdit::TrailingPosition);
  ac_completer->setToolTip(tr("Show Completer Popup."));
  ac_completer->setEnabled(false);
  ac_completer->setVisible(false);

  ac_invalid = addAction(AntiquaCRM::antiquaIcon("dialog-warning"),
                         QLineEdit::TrailingPosition);
  ac_invalid->setToolTip(tr("Invalid content"));
  ac_invalid->setEnabled(false);
  ac_invalid->setVisible(false);

  m_validator = new LineInputValidator(invalidChars(), this);
  setValidator(m_validator);

  connect(ac_completer, SIGNAL(triggered()), SLOT(initCompleter()));
  connect(this, SIGNAL(returnPressed()), SLOT(skipReturnPressed()));
  connect(this, SIGNAL(inputRejected()), SLOT(setVisualFeedback()));
}

const QStringList ALineEdit::invalidChars() {
  return QStringList({"'", "\"", "<", ">", "´", "`"});
}

void ALineEdit::setNumericValidator() {
  setValidator(new QRegularExpressionValidator(AUtil::numericRegExp(), this));
}

void ALineEdit::setArticleValidator() {
  setValidator(new QRegularExpressionValidator(AUtil::articleRegExp(), this));
}

void ALineEdit::initCompleter() {
  QCompleter *m_cpl = completer();
  if (m_cpl != nullptr) {
    QAbstractItemView *m_view = m_cpl->popup();
    m_view->setAlternatingRowColors(true);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_cpl->complete(rect());
    emit sendCompleterShown();
  }
}

void ALineEdit::focusOutEvent(QFocusEvent *event) {
  if (event->lostFocus() && (text().length() > minLength)) {
    emit sendFocusOut();
  }
  QLineEdit::focusOutEvent(event);
}

void ALineEdit::skipReturnPressed() {
  // qDebug("AntiquaCRM::ALineEdit::skipReturnPressed()");
  if (text().length() > minLength)
    setModified(true);
}

void ALineEdit::resetVisualFeedback() { setValidContent(true); }

void ALineEdit::pushStatusTipMessage(const QString &info) {
  QStatusTipEvent _event(info);
  QApplication::sendEvent(this, &_event);
}

void ALineEdit::copyIntoClipboard() {
  const QString _data = text().trimmed();
  if (_data.length() < 4)
    return;

  QApplication::clipboard()->setText(_data, QClipboard::Clipboard);
  pushStatusTipMessage(tr("Copied data into clipboard."));
}

void ALineEdit::setMinLength(int length) {
  minLength = length;
  emit sendMinLengthChanged(minLength);
}

void ALineEdit::setIconWarning(bool b) {
  ac_invalid->setEnabled(b);
  ac_invalid->setVisible(b);
  setClearButtonEnabled(!b);
}

void ALineEdit::setValidContent(bool b) {
  const QString _css = (b) ? "" : "QLineEdit {selection-background-color:red;}";
  setStyleSheet(_css);
  if (!_css.isEmpty() && !text().isEmpty())
    pushStatusTipMessage(tr("Invalid content detected."));
}

void ALineEdit::setVisualFeedback(int timeout) {
  if (hasFocus())
    setValidContent(false);

  QTimer::singleShot(timeout, this, SLOT(resetVisualFeedback()));
}

int ALineEdit::getMinLength() { return minLength; }

void ALineEdit::setValidation(AntiquaCRM::ALineEdit::InputValidator type) {
  switch (type) {
  case (InputValidator::STRINGS):
    setValidator(m_validator);
    break;

  case (InputValidator::ARTICLE):
    setArticleValidator();
    break;

  case (InputValidator::NUMERIC):
    setNumericValidator();
    break;

  default: {
    setValidator(m_validator);
  } break;
  };
}

void ALineEdit::setCompleterAction(bool enabled) {
  ac_completer->setEnabled(enabled);
  ac_completer->setVisible(enabled);
}

void ALineEdit::setLineEditProperties(const QSqlField &prop) {
  if (prop.metaType().id() != QMetaType::QString)
    return;

  int _length = prop.length();
  if (_length > 0) {
    QString _txt(tr("Max allowed length") + " ");
    _txt.append(QString::number(_length));
    _txt.append(".");

    setPlaceholderText(_txt);
    setMaxLength(_length);
  }

  if (prop.requiredStatus() == QSqlField::Required) {
    setClearButtonEnabled(false);
    if (!prop.defaultValue().isNull()) {
      QString _txt = prop.defaultValue().toString();
      if (text().isEmpty() && _txt.length() > 0)
        setText(_txt);
    }
  }

  if (validator() == nullptr)
    setValidation(InputValidator::DEFAULT);
}

int ALineEdit::length() { return text().trimmed().length(); }

const QString ALineEdit::getArticleNumber() {
  QString _buffer = text().trimmed();
  ASettings cfg(this);
  int _len = cfg.value("image/zerofill", 8).toInt();
  QRegularExpressionMatch match = AUtil::articleRegExp().match(_buffer);
  if (match.hasMatch())
    return _buffer.rightJustified(_len, '0');

  return QString();
}

} // namespace AntiquaCRM
