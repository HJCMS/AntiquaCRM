// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerssearchlineedit.h"

CustomersSearchLineEdit::CustomersSearchLineEdit(QWidget *parent)
    : SearchLineEdit{parent} {
  setValidation(SearchLineEdit::Strings);
  setPlaceholderText(tr("Search customers"));
  QString searchTip = tr("Press CTRL+Shift+S to focus in.");
  setToolTip(searchTip);
}

const QString CustomersSearchLineEdit::getSearch() {
  QString txt = text().trimmed();
  txt.replace("'", "");
  return txt;
}
