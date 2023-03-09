// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICEEDIT_H
#define DELIVERSERVICEEDIT_H

#include <QComboBox>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QObject>
#include <QWidget>
#include <AntiquaWidgets>

class DeliverItem;
class DeliverServiceBox;

/**
 * @ingroup utils
 * @brief Deliver Service Editor Widget
 */
class DeliverServiceEdit : public QWidget {
  Q_OBJECT

private:
  IntSpinBox *d_cid;
  DeliverServiceBox *d_srv;
  LineEdit *d_class;
  LineEdit *d_definition;
  LineEdit *d_description;
  EditInfoLabel *d_infopage;
  EditInfoLabel *d_website;
  BoolBox *d_international;
  BoolBox *d_default;
  PriceEdit *d_price;
  AntiquaDateInfo *d_changed;

  QLabel *info(const QString &);

private Q_SLOTS:
  void clearFields();

Q_SIGNALS:
  void message(const QString &str);

public Q_SLOTS:
  void createSubEntry(int id);
  void createNewEntry();
  void setServiceBoxFocus();

public:
  explicit DeliverServiceEdit(QWidget *parent = nullptr);
  const DeliverItem getSaveData();
  void addDeliverServiceEntry(const QMap<QString, QVariant> &entry);
};

#endif // DELIVERSERVICEEDIT_H
