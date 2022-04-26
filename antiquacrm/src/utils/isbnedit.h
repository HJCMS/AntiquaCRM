// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNEDIT_UTILS_H
#define ISBNEDIT_UTILS_H

#include <QtCore/QString>
#include <QtGui/QRegExpValidator>
#include <QtGui/QIcon>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <UtilsMain>

/**
 @brief Klasse für ISBN/EAN nach ISO 2108
*/
class IsbnEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLineEdit *m_isbn;
  QCompleter *m_completer;
  QPushButton *m_query;
  QLabel *m_info;
  QRegExpValidator *m_validator;
  void setButtonText(int);
  bool patternMatching(const QString &);

private Q_SLOTS:
  void feedbackIcon(bool);
  void isbnChanged(const QString &);

Q_SIGNALS:
  void isbnIsValid(bool);
  void clicked();

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit IsbnEdit(QWidget *parent = nullptr);
  bool isValid();
  const QVariant value();
  const QString notes();
};

#endif // ISBNEDIT_UTILS_H
