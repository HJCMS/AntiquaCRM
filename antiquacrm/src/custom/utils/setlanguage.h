// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SETLANGUAGE_UTILS_H
#define SETLANGUAGE_UTILS_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QVariant>
#include <QComboBox>
#include <QWidget>

#include <UtilsMain>

/**
   @brief ISO639-1 Language Code
 */
class SetLanguage final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_comboBox;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit SetLanguage(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // SETLANGUAGE_UTILS_H
