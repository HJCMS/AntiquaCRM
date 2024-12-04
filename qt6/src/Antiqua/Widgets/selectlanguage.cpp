// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectlanguage.h"
#include "antiquaicon.h"

#include <QIcon>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLocale>

namespace AntiquaCRM
{

SelectLanguage::SelectLanguage(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectLanguage::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SelectLanguage::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setLength(3);
  setRestrictions(_f);

  m_edit->setToolTip(tr("Language"));
  m_edit->setWithoutDisclosures();
  m_edit->addItem(tr("German"), "de");
  m_edit->addItem(tr("European"), "eu");
  m_edit->addItem(tr("Czech"), "cz");
  m_edit->addItem(tr("Danish"), "dk");
  m_edit->addItem(tr("English"), "en");
  m_edit->addItem(tr("Spanish"), "es");
  m_edit->addItem(tr("Finnish"), "fi");
  m_edit->addItem(tr("French"), "fr");
  m_edit->addItem(tr("Italian"), "it");
  m_edit->addItem(tr("Dutch"), "nl");
  m_edit->addItem(tr("Norwegian"), "pl");
  m_edit->addItem(tr("Portuguese"), "pt");
  m_edit->addItem(tr("Slovenian"), "si");
  m_edit->addItem(tr("Swedish"), "se");

  for (int r = 0; r < m_edit->count(); r++) {
    QString _c = m_edit->itemData(r, Qt::UserRole).toString();
    if (_c.length() == 2) {
      const QIcon _icon = AntiquaCRM::flagIcon((_c == "en") ? "gb" : _c);
      m_edit->setItemData(r, _icon, Qt::DecorationRole);
    }
  }

  setWindowModified(false);
}

void SelectLanguage::setValue(const QVariant& value) {
  QString _find = value.toString().trimmed().toLower();
  int _index = m_edit->findData(_find, Qt::UserRole, Qt::MatchExactly);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);

  // Deprecated definition import
  if (_find.contains("_")) {
    qWarning("Deprecated ISO language import, convert to bcp47.");
    QStringList _l = _find.split("_");
    _index = m_edit->findData(_l.last().toLower(), Qt::UserRole, Qt::MatchExactly);
    if (_index > 0)
      m_edit->setCurrentIndex(_index);
  }
}

void SelectLanguage::setFocus() {
  m_edit->setFocus();
}

void SelectLanguage::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectLanguage::setRestrictions(const QSqlField& field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectLanguage::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void SelectLanguage::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectLanguage::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectLanguage::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SelectLanguage::getValue() {
  int _index = m_edit->currentIndex();
  if (_index == 0)
    return QString("xx");

  return m_edit->itemData(_index, Qt::UserRole).toString();
}

const QString SelectLanguage::getBCP47Name() {
  return getValue().toString();
}

const QString SelectLanguage::popUpHints() {
  return tr("Language field is required and must set.");
}

const QString SelectLanguage::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
