// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchlineedit.h"
#include "version.h"
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtCore/QRegExp>
#include <QtCore/QStringListModel>

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit{parent} {
  setObjectName("SearchLineEdit");
  setPlaceholderText(tr("Search Books"));

  QRegExp reg;
  reg.setPattern("^\\S{2}.+");
  m_validator = new QRegExpValidator(reg, this);
  setValidator(m_validator);

  QStringList p_keywords;
  // TODO SQL Query last_inserts()
  p_keywords << "1000 Jahre"
             << "1000 Fragen"
             << "1000 Dinge";

  m_completer = new QCompleter(p_keywords, this);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setMaxVisibleItems(2);
  setCompleter(m_completer);

  connect(this, SIGNAL(textChanged(const QString &)), this,
          SLOT(prepareQuery(const QString &)));
}

void SearchLineEdit::prepareQuery(const QString &str) {
  QString buf(str);
  buf = buf.trimmed();
  if (buf.length() >= 2) {
    emit inputChanged(buf);
  }
}
