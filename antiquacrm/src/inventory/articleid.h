// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ARTICLEID_H
#define ARTICLEID_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>

class ArticleID : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged)

private:
  QLabel *id_label;
  bool required = false;
  bool modified = false;

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  /**
   @brief Artikel ID setzen
  */
  void setValue(const QVariant &);

  /**
   @brief Eingabefeld zurücksetzen
  */
  Q_INVOKABLE void reset();

  /**
   @brief Artikel ID setzt Modifiziert
  */
  Q_INVOKABLE void setModified(bool b);

public:
  explicit ArticleID(QWidget *parent = nullptr);

  /**
   @brief Artikel ID Anforderung setzen.
  */
  void setRequired(bool b);

  /**
   @brief Artikel ID ist gefordert?
  */
  bool isRequired();

  /**
   @brief Artikel ID wurde modifiziert
  */
  Q_INVOKABLE bool hasModified();

  /**
   @brief Artikel ID Text
  */
  const QString text();

  /**
   @brief Aktuelle Artikel ID
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

#endif // ARTICLEID_H
