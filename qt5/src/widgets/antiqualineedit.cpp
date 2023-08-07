// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiqualineedit.h"

#include <QDebug>

AntiquaInputValidator::AntiquaInputValidator(QLineEdit *parent)
    : QValidator{parent} {}

const QStringList AntiquaInputValidator::checkList() {
  return QStringList({"'", "\"", "´", "<", ">", "´", "`"});
}

QValidator::State AntiquaInputValidator::validate(QString &input,
                                                  int &pos) const {
  Q_UNUSED(pos);
  foreach (QString _k, checkList()) {
    if (input.contains(_k))
      return QValidator::Invalid;
  }
  return QValidator::Acceptable;
}

AntiquaLineEdit::AntiquaLineEdit(QWidget *parent)
    : QLineEdit{parent}, keyEnterEvent{false} {
  QSizePolicy sp(QSizePolicy::MinimumExpanding, /* klein halten */
                 QSizePolicy::Fixed, QSizePolicy::LineEdit);
  setClearButtonEnabled(true);
  setMinimumWidth(30);
  setSizePolicy(sp);

  m_validator = new AntiquaInputValidator(this);
  setValidator(m_validator);

  connect(this, SIGNAL(textChanged(const QString &)),
          SLOT(checkInput(const QString &)));
  connect(this, SIGNAL(returnPressed()), SLOT(checkEnterEventPressed()));
}

void AntiquaLineEdit::checkInput(const QString &str) {
  if (str.contains("'") || str.contains("\"") || str.contains("´")) {
    setStyleSheet("QLineEdit {selection-background-color:red;}");

    return;
  }
  setStyleSheet(QString());
}

void AntiquaLineEdit::focusInEvent(QFocusEvent *event) {
  if (event->type() == QEvent::FocusIn) {
    if (completer() != nullptr && text().isEmpty() &&
        completer()->completionCount() > 1) {
      completer()->complete();
    }
  }
  QLineEdit::focusInEvent(event);
}

void AntiquaLineEdit::checkEnterEventPressed() {
  if (keyEnterEvent) {
    // emit returnPressed();
    qInfo("Enter key event enabled!");
    return;
  }
  qInfo("Enter key event disabled!");
}

void AntiquaLineEdit::setEnableKeyEnterEvent(bool b) {
  keyEnterEvent = b;
  emit sendKeyEnterEventChanged();
}

bool AntiquaLineEdit::keyEnterEventEnabled() { return keyEnterEvent; }
