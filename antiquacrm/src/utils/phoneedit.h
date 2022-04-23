// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PHONEEDIT_UTILS_H
#define PHONEEDIT_UTILS_H

#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <UtilsMain>

class PhoneEdit : public UtilsMain {
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
    @note Wir Escapen wegen der Datenbanksuche nicht!
    @return QRegExp
  */
  static const QRegExp pcre() { return QRegExp("([\\d\\s]+)"); }

protected Q_SLOTS:
  /**
    @brief Kontrolliert die Eingabe
    @param str - Aktuelle Zeicheneingabe
   */
  void inputChanged(const QString &str);

public Q_SLOTS:
  /**
   @brief Rufnummer setzen
  */
  void setValue(const QVariant &);

  /**
   @brief Eingabefeld zurücksetzen
  */
  Q_INVOKABLE void reset();

public:
  explicit PhoneEdit(QWidget *parent = nullptr);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // PHONEEDIT_UTILS_H
