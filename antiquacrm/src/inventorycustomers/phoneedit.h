// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PHONEEDIT_H
#define PHONEEDIT_H

#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class PhoneEdit : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged)

private:
  QLabel *m_label;
  QLineEdit *id_phone;
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
  static const QRegExp pcre() { return QRegExp("([\\d\\s]+)"); }

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
   @brief Rufnummer setzen
  */
  void setValue(const QVariant &);

  /**
   @brief Eingabefeld zurücksetzen
  */
  Q_INVOKABLE void reset();

  /**
   @brief Rufnummer setzt Modifiziert
  */
  Q_INVOKABLE void setModified(bool b);

public:
  explicit PhoneEdit(QWidget *parent = nullptr);

  /**
   @brief Rufnummer Anforderung setzen.
  */
  void setRequired(bool b);

  /**
   @brief Rufnummer ist gefordert?
  */
  bool isRequired();

  /**
   @brief Rufnummer wurde modifiziert
  */
  Q_INVOKABLE bool hasModified();

  /**
   @brief Rufnummer Text
  */
  const QString text();

  /**
   @brief Aktuelle Rufnummer
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

#endif // PHONEEDIT_H
