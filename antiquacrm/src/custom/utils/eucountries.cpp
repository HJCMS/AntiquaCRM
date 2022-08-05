// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "eucountries.h"

#include <QDebug>
#include <QDomElement>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLocale>

EUCountries::EUCountries(const QString &name) : QDomDocument{name} {
  if (!init()) {
    qWarning("Can't open EU Countries XML File!");
  }
}

const QDir EUCountries::sourcePath() const {
  QString p = QDir::currentPath();
  p.append(QDir::separator());

#ifndef ANTIQUA_DEVELOPEMENT
#ifdef ANTIQUA_LSB_DATADIR
  p = QString(ANTIQUA_LSB_DATADIR);
#endif
#endif

  p.append(QDir::separator());
  p.append("xml");
  return QDir(p);
}

bool EUCountries::init() {
  QFileInfo info(sourcePath(), "eu_countries.xml");
  if (!info.isReadable())
    return false;

  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QString err;
    int l;
    if (!setContent(&fp, &err, &l)) {
      qDebug() << Q_FUNC_INFO << err << l << Qt::endl;
      return false;
    }
    fp.close();
  }
  if (documentElement().hasChildNodes()) {
    p_itemsList = documentElement().elementsByTagName("item");
  }
  return (p_itemsList.size() > 0);
}

const QDomNodeList EUCountries::items() { return p_itemsList; }

const QString EUCountries::countryCode(const QString &text) {
  if (p_itemsList.count() < 1)
    return QString();

  for (int i = 0; i < p_itemsList.count(); i++) {
    QDomElement e = p_itemsList.at(i).toElement();
    if (e.firstChild().nodeValue() == text)
      return e.attribute("countryCode");
  }
  return QString();
}

const QString EUCountries::name(const QString &countryCode) {
  if (p_itemsList.count() < 1)
    return QString();

  for (int i = 0; i < p_itemsList.count(); i++) {
    QDomElement e = p_itemsList.at(i).toElement();
    if (e.attribute("countryCode") == countryCode.toUpper())
      return e.firstChild().nodeValue();
  }
  return QString();
}

EUCountryBox::EUCountryBox(QWidget *parent)
    : UtilsMain{parent}, p_xml("itemlist") {
  setObjectName("eu_country_box");
  setToolTip(tr(""));

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_box = new QComboBox(this);
  layout->addWidget(m_box);

  int c = 0;
  m_box->insertItem(c, tr("No European Country"));
  m_box->setItemData(c++, QString(), Qt::UserRole);

  QLocale lc = QLocale::system();

  QDomNodeList list = p_xml.items();
  for (int i = 0; i < list.count(); i++) {
    QDomElement e = list.at(i).toElement();
    QString attr = e.attribute("countryCode").toLower();
    QString info = e.firstChild().nodeValue();
    m_box->insertItem(c, info);
    m_box->setItemData(c, attr, Qt::UserRole);
    if (attr == lc.bcp47Name())
      m_box->setCurrentIndex(c);

    c++;
  }

  setLayout(layout);
  setModified(false);
  setRequired(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(currentIndexChanged(int)));
}

void EUCountryBox::currentIndexChanged(int) { setModified(true); }

void EUCountryBox::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void EUCountryBox::findCountry(const QString &country) {
  QString search = country.trimmed();
  if (country.contains("/")) {
    search = search.split("/").first();
  }
  // Schritt: Volltextsuche
  int index = m_box->findData(search, Qt::DisplayRole, Qt::MatchStartsWith);
  if (index > 0) {
    m_box->setCurrentIndex(index);
    setModified(true);
  }
  qDebug() << Q_FUNC_INFO << country << index << objectName();
}

void EUCountryBox::setFocus() { m_box->setFocus(); }

void EUCountryBox::setValue(const QVariant &val) {
  QString search = val.toString().toLower();
  // Schritt: 1 nach IETF BCP 47 suchen.
  int index = m_box->findData(search, Qt::UserRole, Qt::MatchExactly);
  if (index > 0) {
    m_box->setCurrentIndex(index);
  } else {
    // Schritt: 2 Volltextsuche
    index = m_box->findData(search, Qt::DisplayRole, Qt::MatchStartsWith);
    if (index > 0) {
      m_box->setCurrentIndex(index);
    }
  }
  setModified((index > 0));
}

const QVariant EUCountryBox::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole);
}

bool EUCountryBox::isValid() { return true; }

void EUCountryBox::setInfo(const QString &info) {
  QString txt(info.trimmed());
  m_info->setText(txt + ":");
  m_box->setToolTip(info);
}

const QString EUCountryBox::info() { return m_box->toolTip(); }

const QString EUCountryBox::notes() {
  return tr("a valid Country selection is required!");
}
