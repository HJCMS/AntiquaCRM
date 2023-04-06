// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"

#include <QAbstractItemView>
#include <QCompleter>
#include <QDebug>
#include <QIcon>
#include <QRegularExpressionValidator>
#include <QValidator>

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent} {
  setClearButtonEnabled(true);
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
  const QRegularExpression pattern("^[0-9]+$");
  setValidator(new QRegularExpressionValidator(pattern, this));
}

void ALineEdit::setArticleValidator() {
  const QRegularExpression pattern("^([0-9]{1,9}[\\,]?)+$");
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
  }
}

void ALineEdit::focusOutEvent(QFocusEvent *event) {
  if (event->lostFocus() && (text().length() > 2)) {
    emit sendFocusOut();
  }
  QLineEdit::focusOutEvent(event);
}

void ALineEdit::skipReturnPressed() {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug("AntiquaCRM::ALineEdit::skipReturnPressed()");
#endif
  if (text().length() > 0)
    setModified(true);
}

void ALineEdit::isValidContent(bool b) {
  QString _css;
  if (!b)
    _css = "QLineEdit {selection-background-color:red;}";

  setStyleSheet(_css);
}

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
