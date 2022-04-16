// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SETLANGUAGE_H
#define SETLANGUAGE_H

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>

/**
   @brief ISO639-1 Language Code
 */
class SetLanguage : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  bool modified;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  void setModified(bool b = true);
  Q_INVOKABLE void reset();

public:
  explicit SetLanguage(QWidget *parent = nullptr);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
};

#endif // SETLANGUAGE_H
