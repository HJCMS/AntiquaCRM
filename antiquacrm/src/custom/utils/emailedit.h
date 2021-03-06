// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EMAILEDIT_UTILS_H
#define EMAILEDIT_UTILS_H

#include <QObject>
#include <QRegExp>
#include <QVariant>
#include <QRegExpValidator>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include <UtilsMain>

class EMailEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_label;
  QLineEdit *m_mail;

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
  const QRegExp pcre();

protected Q_SLOTS:
  /**
    @brief Kontrolliert die Eingabe
    @param str - Aktuelle Zeicheneingabe
   */
  void inputChanged(const QString &str);

public Q_SLOTS:
  /**
   @brief E-Mail setzen
  */
  void setValue(const QVariant &);

  /**
   @brief Eingabefeld zurücksetzen
  */
  Q_INVOKABLE void reset();

  void setFocus();

public:
  explicit EMailEdit(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // EMAILEDIT_UTILS_H
