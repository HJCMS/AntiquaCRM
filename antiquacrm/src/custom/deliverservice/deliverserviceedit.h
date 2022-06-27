// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICEEDIT_H
#define DELIVERSERVICEEDIT_H

#include <QComboBox>
#include <QMap>
#include <QList>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <Utils>

class DeliverItem;

class DeliverServiceEdit : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  DeliveryService *d_srv;
  StrLineEdit *d_class;
  StrLineEdit *d_definition;
  StrLineEdit *d_description;
  StrLineEdit *d_infopage;
  StrLineEdit *d_website;
  BoolBox *d_international;
  PriceEdit *d_price;
  DateTimeDisplay *d_changed;

  QLabel *info(const QString &);

Q_SIGNALS:
  void message(const QString &str);

public:
  explicit DeliverServiceEdit(QWidget *parent = nullptr);
  const DeliverItem getSaveData();
  void addDeliverServiceEntry(const QMap<QString, QVariant> &entry);
};

#endif // DELIVERSERVICEEDIT_H
