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
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QCompleter *m_completer;
  QRegExpValidator *m_validator;

private Q_SLOTS:
  void isbnChanged(const QString &);

Q_SIGNALS:
  void s_isbnIsValid(bool);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit IsbnEdit(QWidget *parent = nullptr);

  Q_INVOKABLE bool hasModified();

  bool isValid();

  const QVariant value();
};

#endif // ISBNEDIT_H
