// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectvat.h"
#include "antiquaicon.h"

namespace AntiquaCRM {

SelectVAT::SelectVAT(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectVAT::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setDefaultValue(AntiquaCRM::SalesTax::TAX_NOT);
  setRestrictions(_f);

  int _c = 1;
  m_edit->insertItem(_c++, AntiquaCRM::antiquaIcon("view-investment"),
                     tr("Without sales tax"), AntiquaCRM::SalesTax::TAX_NOT);

  m_edit->insertItem(_c++, AntiquaCRM::antiquaIcon("view-financial-transfer"),
                     tr("Including sales tax"), AntiquaCRM::SalesTax::TAX_INCL);

  m_edit->insertItem(_c++, AntiquaCRM::antiquaIcon("view-financial-transfer"),
                     tr("With sales tax"), AntiquaCRM::SalesTax::TAX_WITH);

  setWindowModified(false);
}

void SelectVAT::valueChanged(int index) {
  if (index < 1)
    return;

  emit sendInputChanged();
}

void SelectVAT::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::Int) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Reject:" << value;
#endif
    return;
  }
  int _index = m_edit->findData(value.toInt(), Qt::UserRole);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SelectVAT::setFocus() { m_edit->setFocus(); }

void SelectVAT::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectVAT::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectVAT::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void SelectVAT::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectVAT::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectVAT::getType() const { return QMetaType(QMetaType::Int); }

const QVariant SelectVAT::getValue() {
  int _index = m_edit->currentIndex();
  int _status = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::SalesTax>(_status);
}

const QString SelectVAT::popUpHints() { return tr("a valid VAT is required."); }

const QString SelectVAT::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
