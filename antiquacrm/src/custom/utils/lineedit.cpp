// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "lineedit.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QModelIndex>

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit{parent} {
  QSizePolicy sp(QSizePolicy::MinimumExpanding, /* klein halten */
                 QSizePolicy::Fixed, QSizePolicy::LineEdit);
  setMaxLength(80);
  setMinimumWidth(30);
  setSizePolicy(sp);
}

void CustomLineEdit::focusInEvent(QFocusEvent *event) {
  if (event->type() == QEvent::FocusIn) {
    if (completer() != nullptr && text().isEmpty() &&
        completer()->completionCount() > 1) {
      completer()->complete();
    }
  }
  QLineEdit::focusInEvent(event);
}

LineEdit::LineEdit(QWidget *parent, bool enableStretch) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("LineEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_label = new QLabel(this);
  m_label->setAlignment(labelAlignment());
  layout->addWidget(m_label);

  m_edit = new CustomLineEdit(this);
  layout->addWidget(m_edit);

  if (enableStretch) {
    layout->addStretch(1);
  }
  setLayout(layout);

  setRequired(false);
  setModified(false);

  connect(m_edit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(m_edit, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void LineEdit::inputChanged(const QString &str) {
  Q_UNUSED(str)
  setModified(true);
  emit textChanged(str);
}

void LineEdit::setValue(const QVariant &val) {
  p_value = val;
  m_edit->setText(val.toString());
}

void LineEdit::reset() {
  p_value.clear();
  m_edit->clear();
  setModified(false);
}

void LineEdit::setFocus() { m_edit->setFocus(); }

void LineEdit::setPasswordInput(bool b) {
  p_passwordInput = b;
  if (b) {
    m_edit->setInputMethodHints(Qt::ImhHiddenText);
    m_edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  }
}

bool LineEdit::isPasswordInput() { return p_passwordInput; }

void LineEdit::restrictDisplay(int length, int width) {
  m_edit->setMaxLength(length);
  m_edit->setMaximumWidth(width);
}

void LineEdit::addCompleter(const QStringList &list) {
  m_completer = new QCompleter(list, m_edit);
  m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  m_edit->setCompleter(m_completer);
  QString counts = QString::number(list.count());
  QString info(tr("Suggestions available"));
  QString pltext = m_edit->placeholderText();
  if (pltext.isEmpty()) {
    pltext = counts + " " + info;
  } else if (!m_edit->property("prop_suggestions").isNull()) {
    pltext = counts + " " + info;
  } else {
    pltext.append(" " + counts + " " + info);
  }
  m_edit->setProperty("prop_suggestions", 1);
  m_edit->setPlaceholderText(pltext);
}

void LineEdit::setInfo(const QString &info) {
  QString label(info);
  label.append(":");
  m_label->setText(label);
  m_edit->setToolTip(info);
}

const QString LineEdit::info() { return m_label->text(); }

void LineEdit::setPlaceholderText(const QString &txt) {
  m_edit->setPlaceholderText(txt);
}

const QVariant LineEdit::value() {
  if (p_passwordInput)
    return m_edit->text().trimmed().toLocal8Bit();

  return m_edit->text().trimmed();
}

bool LineEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

const QString LineEdit::notes() {
  QString msg(m_edit->toolTip());
  msg.append(" ");
  msg.append(tr("is required and can't empty."));
  return msg;
}
