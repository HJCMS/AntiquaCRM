// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNEDIT_H
#define ISBNEDIT_H

#include <QtCore/QString>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

/**
 @brief Klasse für ISBN/EAN nach ISO 2108
*/
class IsbnEdit : public QLineEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  bool required = false;
  QCompleter *m_completer;
  QRegExpValidator *m_validator;

private Q_SLOTS:
  void isbnChanged(const QString &);

Q_SIGNALS:
  void requireChanged();
  void s_isbnIsValid(bool);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void setModified(bool b) { QLineEdit::setModified(b); };
  Q_INVOKABLE void reset();

public:
  explicit IsbnEdit(QWidget *parent = nullptr);
  void setRequired(bool b);
  bool isRequired();
  Q_INVOKABLE bool hasModified();
  bool isValid();
  const QVariant value();
  const QString notes();
};

#endif // ISBNEDIT_H
