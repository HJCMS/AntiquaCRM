// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EUCOUNTRIES_UTILS_H
#define EUCOUNTRIES_UTILS_H

#include <QComboBox>
#include <QDir>
#include <QDomDocument>
#include <QDomNodeList>
#include <QLabel>
#include <QObject>
#include <UtilsMain>

class EUCountries : public QDomDocument {

private:
  QDomNodeList p_itemsList;
  const QDir sourcePath() const;
  bool init();

public:
  EUCountries(const QString &name = QString("itemlist"));
  const QDomNodeList items();
  const QString countryCode(const QString &text);
  const QString name(const QString &countryCode);
};

class EUCountryBox : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  EUCountries p_xml;
  QLabel *m_info;
  QComboBox *m_box;

private Q_SLOTS:
  void currentIndexChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void findCountry(const QString &);
  void setFocus();
  void setValue(const QVariant &);

public:
  EUCountryBox(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // EUCOUNTRIES_UTILS_H
