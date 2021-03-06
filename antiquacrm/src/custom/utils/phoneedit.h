// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PHONEEDIT_UTILS_H
#define PHONEEDIT_UTILS_H

#include <QObject>
#include <QRegExp>
#include <QVariant>
#include <QRegExpValidator>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QWidget>
#include <UtilsMain>

class PhoneEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_label;
  QLineEdit *id_phone;

  /**
    @brief m_validator
    Wird im Header initialisiert
    und verwendet @ref pcre
  */
  QRegExpValidator *m_validator;

  /**
    @brief Wird mehrfach verwendet
    Das aktuell zulässige Eingabeschema für Telefon Nummern.
  */
  const QRegularExpression regexp = QRegularExpression("^(0[\\d]{2}\\s[\\d]{2,4}[\\s\\d]+)$");

private Q_SLOTS:
  /**
    @brief Kontrolliert die Eingabe
    @param str - Aktuelle Zeicheneingabe
   */
  void inputChanged(const QString &str);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit PhoneEdit(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // PHONEEDIT_UTILS_H
