// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storageeditor.h"
#include "applsettings.h"
#include "storagecategory.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QLayout>
#include <QRegularExpressionMatch>

#ifndef SQL_FIELD_REG
#define SQL_FIELD_REG "^([\\w\\d]+)([\\w\\d\\s\\.\\/\\`\\-]+)$"
#endif

StorageEditor::StorageEditor(QWidget *parent)
    : QWidget{parent}, /* Regulärer ausdruck für Datenfelder */
      pattern(SQL_FIELD_REG, QRegularExpression::UseUnicodePropertiesOption) {

  QVBoxLayout *mainlayout = new QVBoxLayout(this);

  QString st = tr("Empty data fields not accepted and reject this operation.");
  QGroupBox *m_edit = new QGroupBox(this);
  m_edit->setTitle(st);
  QGridLayout *edit_layout = new QGridLayout(m_edit);

  edit_layout->addWidget(subTitle(tr("Storage ID")), 0, 0, 1, 1);
  sl_id = new QSpinBox(this);
  sl_id->setObjectName("sl_id");
  sl_id->setRange(0, 9999);
  sl_id->setReadOnly(true);
  edit_layout->addWidget(sl_id, 0, 1, 1, 1, Qt::AlignLeft);

  edit_layout->addWidget(subTitle(tr("Storage")), 0, 2, 1, 1);
  sl_storage = new QLineEdit(this);
  sl_storage->setObjectName("sl_storage");
  edit_layout->addWidget(sl_storage, 0, 3, 1, 1, Qt::AlignLeft);

  edit_layout->addWidget(subTitle(tr("Identifier")), 1, 0, 1, 1);
  sl_identifier = new QLineEdit(this);
  sl_identifier->setObjectName("sl_identifier");
  edit_layout->addWidget(sl_identifier, 1, 1, 1, 3);

  edit_layout->addWidget(subTitle(tr("Location")), 2, 0, 1, 1);
  sl_location = new QLineEdit(this);
  sl_location->setObjectName("sl_location");
  edit_layout->addWidget(sl_location, 2, 1, 1, 3);
  edit_layout->setColumnStretch(2, 1);
  m_edit->setLayout(edit_layout);
  mainlayout->addWidget(m_edit);

  QGroupBox *m_providers = new QGroupBox(this);
  m_providers->setObjectName("provider_bindings");
  m_providers->setTitle(tr("Deprecated AbeBooks (ZVAB) Categories."));
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
}

QLabel *StorageEditor::subTitle(const QString &text) const {
  QLabel *lb = new QLabel(text);
  lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  return lb;
}

bool StorageEditor::check(QLineEdit *w) {
  QString str = w->text();
  str = str.trimmed();
  QRegularExpressionMatch m = pattern.match(str);
  if (m.hasMatch()) {
    return true;
  }
  w->setFocus();
  return false;
}

bool StorageEditor::setAbeBooks() {
  QDir dest = ApplSettings::getDataTarget("xml");
  QString file("categories_abebooks.xml");
  QFileInfo xmlFile(dest, file);
  if (xmlFile.isReadable()) {
    QMap<QString, QString> data;
    QFile fp(xmlFile.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      m_abebooks->restore();
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

bool StorageEditor::setBooklooker() {
  QDir dest = ApplSettings::getDataTarget("xml");
  QString file("categories_booklooker.xml");
  QFileInfo xmlFile(dest, file);
  if (xmlFile.isReadable()) {
    QMap<QString, QString> data;
    QFile fp(xmlFile.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      m_booklooker->restore();
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

const QString StorageEditor::getCategoryId(const QString &name) {
  StorageCategory *box = findChild<StorageCategory *>(name);
  if (box != nullptr)
    return box->getValue();

  return "0";
}

void StorageEditor::setCategory(const QString &name, qint64 cid) {
  StorageCategory *box = findChild<StorageCategory *>(name);
  if (box != nullptr)
    box->setValue(cid);
}

void StorageEditor::setValue(const StorageTable::RowValues &items) {
  sl_id->setValue(items.sl_id);
  sl_storage->clear();
  sl_storage->setText(items.sl_storage);
  sl_identifier->clear();
  sl_identifier->setText(items.sl_identifier);
  sl_location->clear();
  sl_location->setText(items.sl_location);
  m_abebooks->clear();
  setCategory("abebooks", items.sl_zvab_id);
  m_booklooker->clear();
  setCategory("booklooker", items.sl_booklooker_id);
}

void StorageEditor::clear() {
  sl_id->setValue(0);
  sl_storage->clear();
  sl_identifier->clear();
  sl_location->clear();
  m_abebooks->setIndex(0);
  m_booklooker->setIndex(0);
  sl_storage->setFocus();
}

void StorageEditor::initDataset() {
  setBooklooker();
  setAbeBooks();
}

const QString StorageEditor::sqlQuery() {
  QString sql;
  if (check(sl_storage) && check(sl_identifier) && check(sl_location)) {
    int id = sl_id->value();
    if (id < 1) {
      sql.append("INSERT INTO ref_storage_location ");
      sql.append("(sl_storage,sl_identifier,sl_location");
      sql.append(",sl_zvab_id,sl_zvab_name");
      sql.append(",sl_booklooker_id,sl_booklooker_name)");
      sql.append(" VALUES ('");
      sql.append(sl_storage->text().trimmed());
      sql.append("','");
      sql.append(sl_identifier->text().trimmed());
      sql.append("','");
      sql.append(sl_location->text().trimmed());
      sql.append("',");
      sql.append(getCategoryId("abebooks"));
      sql.append(",'");
      sql.append(m_abebooks->getTitle());
      sql.append("',");
      sql.append(getCategoryId("booklooker"));
      sql.append(",'");
      sql.append(m_booklooker->getTitle());
      sql.append("') RETURNING sl_id;");
    } else {
      sql.append("UPDATE ref_storage_location SET ");
      sql.append("sl_storage='");
      sql.append(sl_storage->text().trimmed());
      sql.append("',sl_identifier='");
      sql.append(sl_identifier->text().trimmed());
      sql.append("',sl_location='");
      sql.append(sl_location->text().trimmed());
      sql.append("',sl_zvab_id=");
      sql.append(getCategoryId("abebooks"));
      sql.append(",sl_zvab_name='");
      sql.append(m_abebooks->getTitle());
      sql.append("',sl_booklooker_id=");
      sql.append(getCategoryId("booklooker"));
      sql.append(",sl_booklooker_name='");
      sql.append(m_booklooker->getTitle());
      sql.append("' WHERE sl_id=" + QString::number(id) + ";");
    }
  }
  return sql;
}
