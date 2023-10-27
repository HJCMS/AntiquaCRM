// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordsedit.h"

#include <QLayout>
#include <QList>
#include <QMetaType>

KeywordsEdit::KeywordsEdit(QWidget *parent) : QWidget{parent} {
  p_record = QSqlRecord();
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_id = new AntiquaCRM::NumEdit(this);
  m_id->setObjectName("ci_id");
  m_id->setBuddyLabel(tr("Id"));
  m_id->setRange(0, 999999);
  m_id->setValue(0);
  m_id->setReadOnly(true);
  layout->addWidget(m_id, 0, 0);

  m_usage = new AntiquaCRM::BoolBox(this);
  m_usage->setObjectName("ci_company_usage");
  m_usage->setBuddyLabel(tr("Usage"));
  m_usage->appendStretch(0);
  layout->addWidget(m_usage, 0, 1);

  m_keyword = new AntiquaCRM::TextLine(this);
  m_keyword->setObjectName("ci_name");
  m_keyword->setBuddyLabel(tr("Keyword"));
  layout->addWidget(m_keyword, 1, 0, 1, 2);

  setLayout(layout);

  connect(m_keyword, SIGNAL(sendInputChanged()), SLOT(keywordChanged()));
}

void KeywordsEdit::keywordChanged() {
  const QString _str = m_keyword->getValue().toString().trimmed();
  if (_str.length() > 2)
    emit search(_str);
}

void KeywordsEdit::clear() {
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>();
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::AInputWidget *e = _list.at(i);
    if (e == nullptr)
      continue;

    e->reset();
  }
  p_record.clear();
}

void KeywordsEdit::setData(const QSqlRecord &record) {
  int _count = record.count();
  if (_count < 3)
    return;

  for (int r = 0; r < _count; r++) {
    QSqlField _field = record.field(r);
    AntiquaCRM::AInputWidget *e =
        findChild<AntiquaCRM::AInputWidget *>(_field.name());
    if (e == nullptr)
      continue;

    e->setRestrictions(_field);
    e->setValue(_field.value());
  }
  p_record = record;
}

void KeywordsEdit::changeId(qint64 id) {
  if (id > 0)
    m_id->setValue(id);
}

const QSqlRecord KeywordsEdit::getData(const QString &tableName) {
  QSqlRecord _record;
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>();
  for (int r = 0; r < _list.size(); r++) {
    AntiquaCRM::AInputWidget *e = _list.at(r);
    if (e == nullptr)
      continue;

    if ((e->getValue().metaType().id() == QMetaType::QString) &&
        e->getValue().toString().length() < 2) {
      e->setFocus();
      break;
    }

    QSqlField _field;
    _field.setTableName(tableName);
    _field.setName(e->objectName());
    _field.setValue(e->getValue());
    _field.setRequired(true);
    _record.append(_field);
  }
  return _record;
}
