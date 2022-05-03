// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SERIALID_UTILS_H
#define SERIALID_UTILS_H

#include <QObject>
#include <QVariant>
#include <QFrame>
#include <QLabel>

#include <UtilsMain>

class SerialID : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_infoLabel;
  QLabel *m_serialLabel;

public Q_SLOTS:
  /**
   @brief Artikel ID setzen
  */
  void setValue(const QVariant &);

  /**
   @brief Eingabefeld zurücksetzen
  */
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit SerialID(QWidget *parent = nullptr);

  /**
     @brief Setze Label Infotext
   */
  void setInfo(const QString &);

  /**
   @brief Nehme Label Text
  */
  const QString info();

  /**
   * @brief Aktuelle Artikel ID
   * @warning Es darf keine 0 zurück gegeben werden!
   * @return Positiver Wert oder Null wegen parsen!
  */
  const QVariant value();

  /**
   @brief Datenprüfung
  */
  bool isValid();

  /**
   @brief Fehlernachricht
  */
  const QString notes();
};

#endif // SERIALID_UTILS_H
