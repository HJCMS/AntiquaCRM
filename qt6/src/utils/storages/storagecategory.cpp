// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagecategory.h"

#include <QLayout>

StorageCategory::StorageCategory(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {

  m_box = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_box);

  m_find = new AntiquaCRM::ALineEdit(this);
  m_find->setMinimumWidth(50);
  m_find->setPlaceholderText(tr("Search Category"));
  m_find->setClearButtonEnabled(true);
  layout->addWidget(m_find);

  connect(m_find, SIGNAL(textChanged(const QString &)), this,
          SLOT(search(const QString &)));

  connect(m_find, SIGNAL(returnPressed()), this, SLOT(onKeyEnterPressed()));
}

void StorageCategory::onKeyEnterPressed() {
  // Ignore Key Press Enter
}

void StorageCategory::initData() {
  m_box->clear();
  m_box->setWithoutDisclosures();
  m_find->clear();
}

void StorageCategory::setValue(const QVariant &value) {
  int _cid = value.toInt();
  if (_cid == 0) {
    m_box->setCurrentIndex(0);
    return;
  }

  const QString _scid = QString::number(_cid);
  for (int i = 0; i < m_box->count(); i++) {
    if (m_box->itemData(i, Qt::UserRole).toString() == _scid) {
      m_box->setCurrentIndex(i);
      return;
    }
  }
}

void StorageCategory::setFocus() { m_box->setFocus(); }

void StorageCategory::reset() {
  m_box->setCurrentIndex(0);
  setWindowModified(false);
}

void StorageCategory::search(const QString &str, Qt::ItemDataRole role) {
  if (str.length() < 3)
    return;

  for (int i = 0; i < m_box->count(); i++) {
    QString name = m_box->itemData(i, role).toString();
    if (name.contains(str, Qt::CaseInsensitive)) {
      m_box->setCurrentIndex(i);
      return;
    }
  }
}

void StorageCategory::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void StorageCategory::setInputToolTip(const QString &tip) {
  m_box->setToolTip(tip);
}

void StorageCategory::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  AntiquaCRM::ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_box);
}

bool StorageCategory::isValid() {
  return (isRequired() && m_box->currentIndex() > 0);
}

const QMetaType StorageCategory::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant StorageCategory::getValue() {
  return m_box->itemData(m_box->currentIndex(), Qt::UserRole).toString();
}

const QString StorageCategory::popUpHints() {
  return tr("Missing storage category selection");
}

const QString StorageCategory::statusHints() { return popUpHints(); }

void StorageCategory::addItems(const QMap<QString, QString> &map) {
  QStringList list;
  QMapIterator<QString, QString> it(map);
  while (it.hasNext()) {
    it.next();
    m_box->addItem(it.key(), it.value());
    list << it.key();
  }

  if (list.count() < 1)
    return;

  m_completer = new QCompleter(list, m_find);
  m_completer->setFilterMode(Qt::MatchContains);
  m_find->setCompleter(m_completer);
}

const QPair<QString, QString> StorageCategory::getItem(qint64 cid) {
  QPair<QString, QString> _p;
  int _cid = (cid > 0) ? cid : m_box->currentIndex();
  _p.first = m_box->itemData(_cid, Qt::DisplayRole).toString();
  _p.second = m_box->itemData(_cid, Qt::UserRole).toString();
  return _p;
}

int StorageCategory::itemCount() { return m_box->count(); }
