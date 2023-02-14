// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salutationbox.h"

#include <QJsonArray>
#include <QJsonObject>

SalutationBox::SalutationBox(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Salutation"));
  m_layout->addWidget(m_box);
  loadDataset();

  m_edit = new QLineEdit(this);
  m_edit->setPlaceholderText(withoutDisclosures());
  m_box->setLineEdit(m_edit);

  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

const QString SalutationBox::withoutDisclosures() {
  return tr("Without disclosures");
}

void SalutationBox::loadDataset() {
  m_box->clear();
  m_box->addItem(QString(), QString());
  QMapIterator<QString, QString> it(salutations());
  int i = 1;
  while (it.hasNext()) {
    it.next();
    m_box->insertItem(i, it.key(), it.key());
    m_box->setItemData(i, it.value(), Qt::ToolTipRole);
    i++;
  }
}

void SalutationBox::dataChanged(int) { setModified(true); }

void SalutationBox::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void SalutationBox::setValue(const QVariant &val) {
  QString txt = val.toString();
  int index = m_box->findData(val, Qt::DisplayRole, Qt::MatchExactly);
  if (index > 0)
    m_box->setCurrentIndex(index);
  else if (!txt.isEmpty())
    m_edit->setText(txt);
}

void SalutationBox::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void SalutationBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.length() > withoutDisclosures().length())
    m_edit->setMaxLength(field.length());
}

const QVariant SalutationBox::value() {
  if (m_box->currentIndex() > 0)
    return m_box->currentText();

  QString edit = m_edit->text().trimmed();
  if (edit != withoutDisclosures())
    return edit;

  return QString();
}

bool SalutationBox::isValid() { return true; }

void SalutationBox::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString SalutationBox::info() { return m_box->toolTip(); }

const QString SalutationBox::notes() { return tr("a Title is required."); }

const QMap<QString, QString> SalutationBox::salutations() {
  QMap<QString, QString> map;
  AntiquaCRM::ASharedDataFiles files(AntiquaCRM::ASettings::getDataDir("json"));
  QJsonObject obj = files.getJson("salutations").object();
  QStringList lng = obj.keys();
  if (lng.size() > 0) {
    QJsonArray arr = obj.value(lng.first()).toArray();
    for (int i = 0; i < arr.size(); i++) {
      QJsonObject item = arr[i].toObject();
      map.insert(item.value("title").toString(),
                 item.value("describe").toString());
    }
  }
  return map;
}
