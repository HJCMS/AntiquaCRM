// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef POSTALCODE_H
#define POSTALCODE_H

#include <QtCore/QObject>
#include <QtSql/QSqlQueryModel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#include <UtilsMain>

class PostalCodeModel : public QSqlQueryModel {
  Q_OBJECT

public:
  explicit PostalCodeModel(const QString &table, QObject *parent = nullptr);
  int size();
};

class PostalCode : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_info;
  QLineEdit *m_plz;
  QCompleter *m_completer;
  QComboBox *m_country;
  bool fetchCompleterTables();
  bool setCompleterModel(const QString &table);

private Q_SLOTS:
  void loadDataset(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit PostalCode(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString selectedTableName();
  const QString notes();
};

#endif // POSTALCODE_H
