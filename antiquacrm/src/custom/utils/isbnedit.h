// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNEDIT_UTILS_H
#define ISBNEDIT_UTILS_H

#include <QString>
#include <QRegExpValidator>
#include <QIcon>
#include <QCompleter>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <UtilsMain>

/**
 @brief Klasse für ISBN/EAN nach ISO 2108
*/
class IsbnEdit final : public UtilsMain {
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
  void setFocus();

public:
  explicit IsbnEdit(QWidget *parent = nullptr);
  bool isValid();
  const QVariant value();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ISBNEDIT_UTILS_H
