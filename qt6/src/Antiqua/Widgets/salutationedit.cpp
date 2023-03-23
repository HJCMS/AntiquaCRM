// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salutationedit.h"

#include <QFontMetrics>
#include <QLineEdit>

namespace AntiquaCRM {

SalutationEdit::SalutationEdit(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setToolTip(tr("Salutation"));
  layout->addWidget(m_edit);
  initData();

  QString _info = m_edit->withoutDisclosures();
  QLineEdit *m_lineEdit = new QLineEdit(this);
  m_lineEdit->setPlaceholderText(_info);
  m_edit->setLineEdit(m_lineEdit);

  QFontMetrics _m(m_edit->font());
  qsizetype _w = _m.size(Qt::TextSingleLine, _info).width();
  m_edit->setMinimumWidth(_w + m_edit->font().pointSize());

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SalutationEdit::SalutationEdit::valueChanged(int) {
  setWindowModified(true);
}

void SalutationEdit::initData() {
  m_edit->clear();
  m_edit->addItem(QString(), QString());
  QMapIterator<QString, QString> it(salutations());
  int i = 1;
  while (it.hasNext()) {
    it.next();
    m_edit->insertItem(i, it.key(), it.key());
    m_edit->setItemData(i, it.value(), Qt::ToolTipRole);
    i++;
  }
}

void SalutationEdit::setValue(const QVariant &) {}

void SalutationEdit::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void SalutationEdit::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SalutationEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() != QSqlField::Required) {
    setRequired(false);
    return;
  }

  int _default = field.defaultValue().toInt();
  if (_default != getValue().toInt()) {
    setValue(_default);
  }
  setRequired(true);
}

void SalutationEdit::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SalutationEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SalutationEdit::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant SalutationEdit::getValue() {
  int index = m_edit->currentIndex();
  if (index > 0)
    return m_edit->itemData(index, Qt::UserRole).toString();

  QString edit = m_edit->lineEdit()->text().trimmed();
  if (edit != m_edit->withoutDisclosures())
    return edit;

  return QString();
}

const QString SalutationEdit::popUpHints() { return tr("Salutation __TODO__"); }

const QString SalutationEdit::statusHints() {
  return tr("Salutation __TODO__");
}

const QMap<QString, QString> SalutationEdit::salutations() {
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

} // namespace AntiquaCRM
