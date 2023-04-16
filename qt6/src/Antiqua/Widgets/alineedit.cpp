// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"

#include <QAbstractItemView>
#include <QCompleter>
#include <QDebug>
#include <QIcon>
#include <QRegularExpressionValidator>
#include <QTimer>
#include <QValidator>

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent} {
  setClearButtonEnabled(true);
  setFocusPolicy(Qt::StrongFocus);
  QIcon back("://icons/view-list.png");
  QIcon icon = QIcon::fromTheme("view-list-details", back);
  ac_completer = addAction(icon, QLineEdit::TrailingPosition);
  ac_completer->setToolTip(tr("Show Completer Popup."));
  ac_completer->setEnabled(false);
  ac_completer->setVisible(false);

  connect(ac_completer, SIGNAL(triggered()), SLOT(showCompleter()));
  connect(this, SIGNAL(returnPressed()), SLOT(skipReturnPressed()));
}

void ALineEdit::setTextValidator() {
  const QRegularExpression pattern("^\\S{2}.+");
  setValidator(new QRegularExpressionValidator(pattern, this));
}

void ALineEdit::setNumericValidator() {
  const QRegularExpression pattern("^\\d+$");
  setValidator(new QRegularExpressionValidator(pattern, this));
}

void ALineEdit::setArticleValidator() {
  const QRegularExpression pattern("^(\\d{1,9}[\\,]?)+$");
  setValidator(new QRegularExpressionValidator(pattern, this));
}

void ALineEdit::showCompleter() {
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

void ALineEdit::resetVisualFeedback() { isValidContent(true); }

void ALineEdit::setMinLength(int length) {
  minLength = length;
  emit sendMinLengthChanged(minLength);
}

void ALineEdit::isValidContent(bool b) {
  QString _css;
  if (!b)
    _css = "QLineEdit {selection-background-color:red;}";

  setStyleSheet(_css);
}

void ALineEdit::setVisualFeedback(int timeout) {
  if (hasFocus())
    isValidContent(false);

  QTimer::singleShot(timeout, this, SLOT(resetVisualFeedback()));
}

int ALineEdit::getMinLength() { return minLength; }

void ALineEdit::setValidation(AntiquaCRM::ALineEdit::InputValidator type) {
  switch (type) {
  case (InputValidator::STRINGS):
    setTextValidator();
    break;

  case (InputValidator::ARTICLE):
    setArticleValidator();
    break;

  case (InputValidator::NUMERIC):
    setNumericValidator();
    break;

  default: {
    if (validator() != nullptr)
      setValidator(nullptr); // remove it
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

  setValidation(InputValidator::NOTHING);
}

int ALineEdit::length() { return text().trimmed().length(); }

} // namespace AntiquaCRM
