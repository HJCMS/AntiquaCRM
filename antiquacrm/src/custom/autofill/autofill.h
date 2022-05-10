// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef AUTOFILL_XML_H
#define AUTOFILL_XML_H

#include <QCompleter>
#include <QDomDocument>
#include <QObject>

class AutofillDB : public QDomDocument {
public:
  explicit AutofillDB(const QString &name = QString("itemlist"));
  QStringList itemList();
};

class Autofill : public QObject {
  Q_OBJECT

private:
  const QString findFile(const QString &key) const;

public:
  explicit Autofill(QObject *parent = nullptr);
  QCompleter *loadAutofill(const QString &key);
};

#endif // AUTOFILL_XML_H
