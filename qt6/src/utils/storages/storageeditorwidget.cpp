// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storageeditorwidget.h"
#include "storagecategory.h"

#include <QLayout>
#include <QRegularExpressionMatch>
#include <QSqlField>
#include <QtCore>
#include <QtXml>

#ifndef SQL_FIELD_REG
#define SQL_FIELD_REG "^([\\w\\d]+)([\\w\\d\\s\\.\\/\\`\\-]+)$"
#endif

StorageEditorWidget::StorageEditorWidget(QWidget *parent)
    : QWidget{parent}, /* Regulärer ausdruck für Datenfelder */
      pattern(SQL_FIELD_REG, QRegularExpression::UseUnicodePropertiesOption) {
  setContentsMargins(1, 1, 1, 1);

  Q_INIT_RESOURCE(categories);

  QVBoxLayout *mainlayout = new QVBoxLayout(this);
  mainlayout->setContentsMargins(contentsMargins());

  QString st = tr("Empty data fields not accepted and reject this operation.");
  QGroupBox *m_edit = new QGroupBox(this);
  m_edit->setTitle(st);

  QGridLayout *gridLayout = new QGridLayout(m_edit);
  gridLayout->setContentsMargins(contentsMargins());

  sl_id = new AntiquaCRM::NumEdit(this);
  sl_id->setObjectName("sl_id");
  sl_id->setRange(0, 9999);
  sl_id->setReadOnly(true);
  sl_id->setBuddyLabel(tr("Storage ID"));
  gridLayout->addWidget(sl_id, 0, 0, 1, 2);

  sl_storage = new AntiquaCRM::TextLine(this);
  sl_storage->setObjectName("sl_storage");
  sl_storage->setBuddyLabel(tr("Storage"));
  sl_storage->appendStretch(1);
  gridLayout->addWidget(sl_storage, 0, 2, 1, 2);

  gridLayout->addWidget(subTitle(tr("Identifier")), 1, 0, 1, 1);
  sl_identifier = new AntiquaCRM::TextLine(this);
  sl_identifier->setObjectName("sl_identifier");
  gridLayout->addWidget(sl_identifier, 1, 1, 1, 3);

  gridLayout->addWidget(subTitle(tr("Location")), 2, 0, 1, 1);
  sl_location = new AntiquaCRM::TextLine(this);
  sl_location->setObjectName("sl_location");
  gridLayout->addWidget(sl_location, 2, 1, 1, 3);
  gridLayout->setColumnStretch(2, 1);
  m_edit->setLayout(gridLayout);
  mainlayout->addWidget(m_edit);

  QGroupBox *m_providers = new QGroupBox(this);
  m_providers->setObjectName("provider_bindings");
  m_providers->setTitle(tr("Deprecated Provider categories"));

  QHBoxLayout *provider_layout = new QHBoxLayout(m_providers);
  provider_layout->addWidget(subTitle("AbeBooks"));
  m_abebooks = new StorageCategory(m_providers);
  m_abebooks->setObjectName("abebooks");
  provider_layout->addWidget(m_abebooks);

  provider_layout->addWidget(subTitle("Booklooker"));
  m_booklooker = new StorageCategory(m_providers);
  m_booklooker->setObjectName("booklooker");
  provider_layout->addWidget(m_booklooker);

  m_providers->setLayout(provider_layout);
  mainlayout->addWidget(m_providers);

  setLayout(mainlayout);

  connect(sl_storage, SIGNAL(sendInputChanged()), SLOT(dataModified()));
  connect(sl_identifier, SIGNAL(sendInputChanged()), SLOT(dataModified()));
  connect(sl_location, SIGNAL(sendInputChanged()), SLOT(dataModified()));
  connect(m_abebooks, SIGNAL(sendInputChanged()), SLOT(dataModified()));
  connect(m_booklooker, SIGNAL(sendInputChanged()), SLOT(dataModified()));
}

QLabel *StorageEditorWidget::subTitle(const QString &text) const {
  QLabel *lb = new QLabel(text);
  lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  return lb;
}

bool StorageEditorWidget::check(AntiquaCRM::TextLine *w) {
  QString _str = w->getValue().toString();
  _str = _str.trimmed();
  const QRegularExpressionMatch m = pattern.match(_str);
  if (m.hasMatch())
    return true;

  w->setFocus();
  return false;
}

bool StorageEditorWidget::setAbeBooks() {
  QString file(":xml/categories_abebooks.xml");
  QFileInfo xmlFile(file);
  if (xmlFile.isReadable()) {
    QMap<QString, QString> data;
    QFile fp(file);
    if (fp.open(QIODevice::ReadOnly)) {
      m_abebooks->initData();

      QDomDocument dom;
      if (dom.setContent(&fp, false)) {
        QDomNodeList list = dom.elementsByTagName("item");
        for (int i = 0; i < list.size(); i++) {
          QDomElement e = list.at(i).toElement();
          QString name = e.firstChild().nodeValue().trimmed();
          if (!name.isEmpty())
            data.insert(name, e.attribute("id", "0"));
        }
      }
      fp.close();
      if (data.size() > 0) {
        m_abebooks->addItems(data);
        return true;
      }
    }
  }
  return false;
}

bool StorageEditorWidget::setBooklooker() {
  QString file(":xml/categories_booklooker.xml");
  QFileInfo xmlFile(file);
  if (xmlFile.isReadable()) {
    QMap<QString, QString> data;
    QFile fp(xmlFile.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      m_booklooker->initData();

      QDomDocument dom;
      if (dom.setContent(&fp, false)) {
        QDomNodeList list = dom.elementsByTagName("item");
        for (int i = 0; i < list.size(); i++) {
          QDomElement e = list.at(i).toElement();
          QString name = e.firstChild().nodeValue().trimmed();
          if (!name.isEmpty())
            data.insert(name, e.attribute("id", "0"));
        }
      }
      fp.close();
      if (data.size() > 0) {
        m_booklooker->addItems(data);
        return true;
      }
    }
  }
  return false;
}

void StorageEditorWidget::setCategory(const QString &name, qint64 cid) {
  StorageCategory *box = findChild<StorageCategory *>(name);
  if (box != nullptr)
    box->setValue(cid);
}

void StorageEditorWidget::dataModified() { setWindowModified(true); }

void StorageEditorWidget::setValue(const StorageItemData &items) {
  sl_id->setValue(items.sl_id);
  sl_storage->reset();
  sl_storage->setValue(items.sl_storage);
  sl_identifier->reset();
  sl_identifier->setValue(items.sl_identifier);
  sl_location->reset();
  sl_location->setValue(items.sl_location);
  m_abebooks->reset();
  setCategory("abebooks", items.sl_zvab_id);
  m_booklooker->reset();
  setCategory("booklooker", items.sl_booklooker_id);
  setWindowModified(false);
}

void StorageEditorWidget::setRestrictions(const QSqlRecord &record) {
  for (int c = 0; c < record.count(); c++) {
    const QSqlField _f = record.field(c);
    AntiquaCRM::AInputWidget *w =
        findChild<AntiquaCRM::AInputWidget *>(_f.name());
    if (w == nullptr)
      continue;

    w->setRestrictions(_f);
  }
}

void StorageEditorWidget::clear() {
  sl_id->setValue(0);
  sl_storage->reset();
  sl_identifier->reset();
  sl_location->reset();
  m_abebooks->setValue(0);
  m_booklooker->setValue(0);
  sl_storage->setFocus();
}

void StorageEditorWidget::initDataset() {
  setBooklooker();
  setAbeBooks();
}

const QString StorageEditorWidget::sqlQuery() {
  QString sql;
  if (check(sl_storage) && check(sl_identifier) && check(sl_location)) {
    qint64 _sl_id = sl_id->getValue().toInt();
    if (_sl_id < 1) {
      sql.append("INSERT INTO ref_storage_location ");
      sql.append("(sl_storage,sl_identifier,sl_location");
      sql.append(",sl_zvab_id,sl_zvab_name");
      sql.append(",sl_booklooker_id,sl_booklooker_name)");
      sql.append(" VALUES ('");
      sql.append(sl_storage->getValue().toString());
      sql.append("','");
      sql.append(sl_identifier->getValue().toString());
      sql.append("','");
      sql.append(sl_location->getValue().toString());
      sql.append("',");
      sql.append(m_abebooks->getItem().second);
      sql.append(",'");
      sql.append(m_abebooks->getItem().first);
      sql.append("',");
      sql.append(m_booklooker->getItem().second);
      sql.append(",'");
      sql.append(m_booklooker->getItem().first);
      sql.append("') RETURNING sl_id;");
    } else {
      sql.append("UPDATE ref_storage_location SET ");
      QStringList _list;
      QListIterator<AntiquaCRM::AInputWidget *> _it(
          findChildren<AntiquaCRM::AInputWidget *>());
      while (_it.hasNext()) {
        AntiquaCRM::AInputWidget *mi = _it.next();
        if (mi == nullptr || mi->objectName() == "sl_id")
          continue;

        QString _name = mi->objectName();
        QString _value = mi->getValue().toString();
        if (_value.isEmpty() || !_name.startsWith("sl_"))
          continue;

        _list.append(_name + "='" + _value + "'");
      }
      sql.append(_list.join(","));
      sql.append(",sl_zvab_id=");
      sql.append(m_abebooks->getItem().second);
      sql.append(",sl_zvab_name='");
      sql.append(m_abebooks->getItem().first);
      sql.append("',sl_booklooker_id=");
      sql.append(m_booklooker->getItem().second);
      sql.append(",sl_booklooker_name='");
      sql.append(m_booklooker->getItem().first);
      sql.append("' WHERE sl_id=" + QString::number(_sl_id) + ";");
    }
  }
  return sql;
}
