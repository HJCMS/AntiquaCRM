// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autofill.h"
#include "applsettings.h"

#include <QDebug>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QStandardPaths>
#include <QtCore>

AutofillDB::AutofillDB(const QString &name) : QDomDocument{name} {}

QStringList AutofillDB::itemList() {
  QStringList list;
  QDomNodeList nodes = documentElement().elementsByTagName("item");
  for (int i = 0; i < nodes.length(); i++) {
    QDomNode n = nodes.item(i);
    if (!n.firstChild().nodeValue().isEmpty())
      list << n.firstChild().nodeValue();
  }
  return list;
}

Autofill::Autofill(QObject *parent) : QObject{parent} {}

const QString Autofill::findXmlDataFile(const QString &key) const {
  QString lc(QLocale().bcp47Name());
  QDir p = ApplSettings::getDataTarget();
  p.setPath("xml");
  QString file(key + "_" + lc + ".xml");
  QFileInfo info(p, file);
  if (info.exists())
    return info.filePath();

  qWarning("Autofill: no xml data target found!");
  return QString();
}

QCompleter *Autofill::loadAutofill(const QString &key) {
  QString file = findXmlDataFile(key);
  if (!file.isEmpty()) {
    QStringList list;
    QFile fp(file);
    if (fp.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QString err;
      int l;
      AutofillDB dom;
      if (dom.setContent(&fp, false, &err, &l)) {
        list = dom.itemList();
      } else {
        qDebug() << Q_FUNC_INFO << err << l;
      }
      fp.close();
    }
    QCompleter *cpl = new QCompleter(list, this);
    cpl->setObjectName("autofill_completer");
    cpl->setCompletionMode(QCompleter::PopupCompletion);
    cpl->setFilterMode(Qt::MatchStartsWith);
    cpl->setCaseSensitivity(Qt::CaseInsensitive);
    return cpl;
  }
  return nullptr;
}
