// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salutationedit.h"

#include <AntiquaCRM>
#include <QFontMetrics>
#include <QLineEdit>
#include <QLocale>

namespace AntiquaCRM {

SalutationEdit::SalutationEdit(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setToolTip(tr("Salutation"));

  m_lineEdit = new ALineEdit(this);
  m_lineEdit->setClearButtonEnabled(false);
  m_lineEdit->setPlaceholderText(m_edit->withoutDisclosures());
  m_edit->setLineEdit(m_lineEdit);

  layout->addWidget(m_edit);

  QString _info = m_lineEdit->placeholderText();
  QFontMetrics _m(m_edit->font());
  qsizetype _w = _m.size(Qt::TextSingleLine, _info).width();
  m_edit->setMinimumWidth(_w + m_edit->font().pointSize());

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SalutationEdit::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SalutationEdit::setValue(const QVariant &val) {
  const QString _str = val.toString().trimmed();
  if (_str.isEmpty())
    return;

  int _index = m_edit->findText(_str);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
  else
    m_lineEdit->setText(_str);
}

void SalutationEdit::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void SalutationEdit::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SalutationEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setLength(25);
  setRestrictions(_f);

  m_edit->clear();
  m_edit->addItem(QString(), QString());
  QMapIterator<QString, QString> it(SalutationEdit::map());
  int i = 1;
  while (it.hasNext()) {
    it.next();
    m_edit->insertItem(i, it.key(), it.key());
    m_edit->setItemData(i, it.value(), Qt::ToolTipRole);
    i++;
  }
}

void SalutationEdit::setRestrictions(const QSqlField &field) {
  if (field.length() > 0)
    m_lineEdit->setMaxLength(field.length());

  if (field.requiredStatus() != QSqlField::Required) {
    setRequired(false);
    return;
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

const QMetaType SalutationEdit::getType() const {
  return QMetaType(QMetaType::QString);
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

const QString SalutationEdit::popUpHints() {
  return tr("A salutation is missing!");
}

const QString SalutationEdit::statusHints() { return popUpHints(); }

const QMap<QString, QString> SalutationEdit::map(const QString &bcp47) {
  QMap<QString, QString> _map;
  AntiquaCRM::ASharedDataFiles file(AntiquaCRM::ASettings::getDataDir("json"));
  QJsonObject _salutations = file.getJson("salutations").object();
  QStringList _sections = _salutations.keys();
  const QString _lc = QLocale::system().bcp47Name().toLower();
  if (_sections.size() > 0) {
    const QString _key(_sections.contains(_lc) ? _lc : bcp47);
    QJsonArray _array = _salutations.value(_key).toArray();
    for (int i = 0; i < _array.size(); i++) {
      QJsonObject _object = _array[i].toObject();
      _map.insert(_object.value("title").toString(),
                  _object.value("describe").toString());
    }
  }
  return _map;
}

} // namespace AntiquaCRM
