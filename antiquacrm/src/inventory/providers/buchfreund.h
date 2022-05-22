// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_BUCHFREUND_H
#define INVENTORY_PROVIDERS_BUCHFREUND_H

#include <QJsonDocument>
#include <QListWidget>
#include <QObject>
#include <QRegularExpression>
#include <QSplitter>
#include <QWidget>

#include <Utils>

namespace HJCMS {
class SqlCore;
};

class Buchfreundliste : public QListWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  void addOrder(const QString &, const QDateTime &);

private Q_SLOTS:
  void getItemData(QListWidgetItem *);

public Q_SLOTS:
  void setView(const QJsonDocument &doc);

Q_SIGNALS:
  void itemClicked(const QJsonDocument &);

public:
  explicit Buchfreundliste(QWidget *parent = nullptr);
};

class BuchfreundDisplay : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  EMailEdit *c_email_0;
  LineEdit *c_country;
  LineEdit *c_firstname;
  LineEdit *c_lastname;
  LineEdit *c_location;
  LineEdit *c_street;
  SerialID *c_id;
  PhoneEdit *c_phone_0;
  PostalCode *c_postalcode;
  SerialID *a_article_id;
  LineEdit *a_provider_id;
  IntSpinBox *a_count;
  PriceEdit *a_sell_price;
  QLineEdit *a_title;
  DateTimeEdit *a_modified;
  QPushButton *btn_create_costumer;
  QPushButton *btn_search_costumer;
  QLabel *costumer_info;

  HJCMS::SqlCore *m_sql;

  const QHash<QString, QVariant> createSqlDataset();

  /**
     @brief Wird für QObject::findchild benötigt!
     Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen
     zu finden. Sie sind Identisch mit den SQL Feldern und beginnen bei
     der Tabelle "costumers" immer mit "c_".
  */
  const QRegularExpression p_objPattern = QRegularExpression("^c_[a-z0-3_]+$");

private Q_SLOTS:
  void createCostumer();
  void searchCostumer();

public Q_SLOTS:
  void setContent(const QJsonDocument &);

Q_SIGNALS:
  void s_costumer(int);

public:
  explicit BuchfreundDisplay(QWidget *parent = nullptr);
  void setValue(const QString &objName, const QVariant &value);
};

class Buchfreund : public QSplitter {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString configGroup;
  Buchfreundliste *m_listView;
  BuchfreundDisplay *m_display;

  const QUrl apiQuery(const QString &operation);

  void sendJsonRequest(const QUrl &, const QByteArray &);

  void queryListViewContent();

public Q_SLOTS:
  void getOpenOrders();
  void getOrderContent(const QJsonDocument &);

Q_SIGNALS:
  void s_costumer(int);

public:
  explicit Buchfreund(QWidget *parent = nullptr);
  void testing();
};

#endif // INVENTORY_PROVIDERS_BUCHFREUND_H
