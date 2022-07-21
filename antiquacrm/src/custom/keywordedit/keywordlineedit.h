// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_LINEDIT_H
#define KEYWORD_LINEDIT_H

#include <QObject>
#include <QWidget>
#include <QCompleter>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>

#include <UtilsMain>

class KeywordLineEdit : public UtilsMain
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLineEdit *m_lineEdit;
  QCompleter *m_completer;
  QRegExpValidator *m_validator;
  const QRegExp pattern = QRegExp("([\\w\\d\\,\\s]+)");

public Q_SLOTS:
  void setValue(const QVariant &str);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit KeywordLineEdit(QWidget *parent = nullptr);
  void loadKeywords();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // KEYWORDEDIT_LINEDIT_H
