// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectgenre.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

SelectGenre::SelectGenre(QWidget *parent) : QComboBox{parent} {
  setObjectName("id3_tag_selecter");

  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Without disclosures"));
  setLineEdit(m_search);

  reload();

  connect(m_search, SIGNAL(textChanged(const QString &)),
          SLOT(find(const QString &)));
}

void SelectGenre::find(const QString &s) { qDebug() << Q_FUNC_INFO << s; }

void SelectGenre::reload() {
  clear();
  insertItem(0, QString(), 0);
  int c = 1;
  QJsonObject jso = id3Tags();
  QJsonObject::Iterator it;
  QStringList tags;
  for (it = jso.begin(); it != jso.end(); ++it) {
    QString _k = it.key();
    QJsonValue _v = it.value();
    tags << _k;
    insertItem(c++, _k, _v.toInt());
  }
  m_cpl = new QCompleter(tags, m_search);
  m_cpl->setCaseSensitivity(Qt::CaseInsensitive);
  m_cpl->setFilterMode(Qt::MatchContains);
  setCompleter(m_cpl);
}

const QJsonObject SelectGenre::id3Tags() {
  AntiquaCRM::ASharedDataFiles tpl(AntiquaCRM::ASettings::getDataDir("json"));
  QJsonDocument js = tpl.getJson("id3taglist");
  if (js.isEmpty()) {
    qWarning("Genre Id's not loaded!");
    return QJsonObject();
  }

  QJsonObject genres = js.object();
  if (genres.keys().size() > 0)
    return genres;

  return QJsonObject();
}

const QStringList SelectGenre::completer() {
  static const QRegularExpression _regexp("[\\s\\t\\+\\-\\/]");
  QStringList _list;
  foreach (QString _g, id3Tags().keys()) {
    if (_regexp.match(_g).hasMatch())
      continue;

    _list << _g;
  }
  _list.removeDuplicates();
  _list.sort(Qt::CaseSensitive);
  return _list;
}

const QString SelectGenre::getName() {
  QString current = currentText();
  if (current.length() < 3)
    return QString();

  current.replace("-", ",");
  current.replace(" ", ",");
  return current.trimmed();
}

int SelectGenre::getId() {
  return itemData(currentIndex(), Qt::UserRole).toInt();
}
