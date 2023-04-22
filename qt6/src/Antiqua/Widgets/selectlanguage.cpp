// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectlanguage.h"

#include <QLocale>

namespace AntiquaCRM {

SelectLanguage::SelectLanguage(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
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
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setLength(3);
  setRestrictions(_f);

  m_edit->setToolTip(tr("Language"));
  m_edit->setWithoutDisclosures();
  m_edit->addItem(tr("German"), "de_DE");
  m_edit->addItem(tr("European"), "eu_EU");
  m_edit->addItem(tr("Czech"), "cs_CS");
  m_edit->addItem(tr("Danish"), "da_DA");
  m_edit->addItem(tr("English"), "en_EN");
  m_edit->addItem(tr("Spanish"), "es_ES");
  m_edit->addItem(tr("Finnish"), "fi_FI");
  m_edit->addItem(tr("French"), "fr_FR");
  m_edit->addItem(tr("Italian"), "it_IT");
  m_edit->addItem(tr("Dutch"), "nl_NL");
  m_edit->addItem(tr("Norwegian"), "nn_NN");
  m_edit->addItem(tr("Polish"), "pl_PL");
  m_edit->addItem(tr("Portuguese"), "pt_PT");
  m_edit->addItem(tr("Slovenian"), "sl_SL");
  m_edit->addItem(tr("Swedish"), "sv_SV");

  setWindowModified(false);
}

void SelectLanguage::setValue(const QVariant &value) {
  int index = m_edit->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (index > 0)
    m_edit->setCurrentIndex(index);
}

void SelectLanguage::setFocus() { m_edit->setFocus(); }

void SelectLanguage::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectLanguage::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectLanguage::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectLanguage::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectLanguage::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant SelectLanguage::getValue() {
  int _index = m_edit->currentIndex();
  if (_index == 0) {
    QString _lang = QLocale::system().bcp47Name();
    return _lang.toLower() + "_" + _lang.toUpper();
  } else {
    return m_edit->itemData(_index, Qt::UserRole);
  }
}

const QString SelectLanguage::getBCP47Name() {
  QString _lang = getValue().toString();
  if (_lang.isEmpty())
    return QLocale::system().bcp47Name();
  else
    return _lang.split("_").first().toUpper();
}

const QString SelectLanguage::popUpHints() {
  return tr("Language field is required and must set.");
}

const QString SelectLanguage::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
