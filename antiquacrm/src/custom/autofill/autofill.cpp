// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autofill.h"

#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QStandardPaths>
#include <QtCore>

static const QString dataLocation(const QString &sfile) {
  QStringList list =
      QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  QFileInfo fi;
  foreach (QString l, list) {
    fi.setFile(l + sfile);
    if (fi.exists())
      return l;
  }
  return list.first();
}

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

const QString Autofill::findFile(const QString &key) const {
  QString l(QLocale().bcp47Name());
  QString s(QDir::separator());
  QString suffix(s + "xml" + s + key + "_" + l + ".xml");
  QFileInfo info(QDir::currentPath() + suffix);
  if (info.exists())
    return info.filePath();

  return dataLocation(suffix);
}

QCompleter *Autofill::loadAutofill(const QString &key) {
  QString file = findFile(key);
  if (!file.isEmpty()) {
    // qDebug() << Q_FUNC_INFO << file;
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
