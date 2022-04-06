// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SETLANGUAGE_H
#define SETLANGUAGE_H

#include <QtCore/QList>
#include <QtCore/QHash>
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

public:
  explicit SetLanguage(QWidget *parent = nullptr);
  void setValue(const QVariant &);
  const QVariant value();
};

#endif // SETLANGUAGE_H
