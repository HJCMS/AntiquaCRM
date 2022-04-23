// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EMAILEDIT_H
#define EMAILEDIT_H

#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class EMailEdit : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged)

private:
  QLabel *m_label;
  QLineEdit *m_mail;
  bool required = false;
  bool modified = false;

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

private Q_SLOTS:
  /**
    @brief Kontrolliert die Eingabe
    @param str - Aktuelle Zeicheneingabe
   */
  void inputChanged(const QString &str);

  /**
    @brief skipReturnPressed
    @warning Enter soll das Formular nicht absenden!
  */
  void skipReturnPressed();

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  /**
   @brief E-Mail setzen
  */
  void setValue(const QVariant &);

  /**
   @brief Eingabefeld zurücksetzen
  */
  Q_INVOKABLE void reset();

  /**
   @brief E-Mail setzt Modifiziert
  */
  Q_INVOKABLE void setModified(bool b);

public:
  explicit EMailEdit(QWidget *parent = nullptr);

  /**
   @brief E-Mail Anforderung setzen.
  */
  void setRequired(bool b);

  /**
   @brief E-Mail ist gefordert?
  */
  bool isRequired();

  /**
   @brief E-Mail wurde modifiziert
  */
  Q_INVOKABLE bool hasModified();

  /**
   @brief E-Mail Text
  */
  const QString text();

  /**
   @brief Aktuelle E-Mail
  */
  const QVariant value();

  /**
   @brief Datenprüfung
  */
  bool isValid();

  /**
     @brief Info Text
  */
  void setInfoText(const QString &);

  /**
   @brief Fehlernachricht
  */
  const QString notes();
};

#endif // EMAILEDIT_H
