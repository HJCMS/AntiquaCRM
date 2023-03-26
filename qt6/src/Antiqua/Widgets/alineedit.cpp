// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"

#include <QDebug>

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent} {
  setClearButtonEnabled(true);
  connect(this, SIGNAL(returnPressed()), SLOT(skipReturnPressed()));
}

void ALineEdit::skipReturnPressed() {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug("AntiquaCRM::ALineEdit::skipReturnPressed()");
#endif
  if (text().length() > 0)
    setModified(true);
}

void ALineEdit::isValidContent(bool b) {
  if (b)
    setStyleSheet(QString());
  else
    setStyleSheet("QLineEdit {selection-background-color: red;}");
}

void ALineEdit::setLineEditProperties(const QSqlField &prop) {
  if (prop.metaType().id() != QMetaType::QString)
    return;

  int max_chars = prop.length();
  if (max_chars > 0) {
    QString _txt(tr("Max allowed length") + " ");
    _txt.append(QString::number(max_chars));
    _txt.append(".");

    setPlaceholderText(_txt);
    setMaxLength(max_chars);
  }

  if (prop.requiredStatus() == QSqlField::Required) {
    setClearButtonEnabled(false);
    if (!prop.defaultValue().isNull()) {
      QString _txt = prop.defaultValue().toString();
      if (text().isEmpty() && _txt.length() > 0)
        setText(_txt);
    }
  }
}

} // namespace AntiquaCRM
