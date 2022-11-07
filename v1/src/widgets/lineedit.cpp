// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "lineedit.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QModelIndex>

LineEdit::LineEdit(QWidget *parent, bool enableStretch) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_layout->addWidget(m_edit);
  if (enableStretch) {
    m_layout->addStretch(1);
  }
  setRequired(false);
  setModified(false);
  connect(m_edit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));
  connect(m_edit, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void LineEdit::loadDataset() {}

void LineEdit::inputChanged(const QString &str) {
  Q_UNUSED(str)
  setModified(true);
  emit textChanged(str);
}

void LineEdit::setValue(const QVariant &val) {
  m_edit->setText(val.toString());
}

void LineEdit::reset() {
  m_edit->clear();
  setModified(false);
}

void LineEdit::setFocus() { m_edit->setFocus(); }

void LineEdit::setProperties(const QSqlField &field) {
  if (!field.isValid())
    return;

  if (field.type() == QVariant::String && field.length() > 0) {
    m_edit->setMaxLength(field.length());

    QString txt(tr("Max allowed length") + " ");
    txt.append(QString::number(field.length()));
    m_edit->setPlaceholderText(txt);
  }

  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
    m_edit->setClearButtonEnabled(false);
  }
}

void LineEdit::setPasswordInput(bool b) {
  p_passwordInput = b;
  if (b) {
    m_edit->setInputMethodHints(Qt::ImhHiddenText);
    m_edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  }
}

bool LineEdit::isPasswordInput() { return p_passwordInput; }

void LineEdit::setCompleter(const QStringList &list) {
  if (list.size() < 1) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Remove Completer";
#endif
    m_edit->setCompleter(nullptr);
    return;
  }

  m_completer = new QCompleter(list, m_edit);
  m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  if (list.size() < 50)
    m_completer->setFilterMode(Qt::MatchContains);

  m_edit->setCompleter(m_completer);
}

void LineEdit::setInfo(const QString &info) {
  if (info.isEmpty())
    return;

  QString label(info);
  label.append(":");
  m_label->setVisible(true);
  m_label->setText(label);
  m_edit->setToolTip(info);
}

const QString LineEdit::info() {
  if (m_label->isVisible())
    return m_edit->toolTip();
  else
    return toolTip();
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
  QString msg(info() + " ");
  msg.append(tr("is required and can't empty."));
  return msg;
}
