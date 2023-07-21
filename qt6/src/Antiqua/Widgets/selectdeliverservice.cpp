// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectdeliverservice.h"

namespace AntiquaCRM {

SelectDeliverService::SelectDeliverService(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setMinimumContentsLength(10);
  layout->addWidget(m_edit);

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectDeliverService::valueChanged(int index) {
  setWindowModified(true);
  emit sendInputChanged();
  if (index > 0)
    emit sendSelectedService(index);
}

void SelectDeliverService::setValue(const QVariant &value) {
  if (m_edit->count() < 1)
    return;

  int _index = m_edit->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SelectDeliverService::setFocus() { m_edit->setFocus(); }

void SelectDeliverService::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectDeliverService::initData() {
  QString _sql("SELECT d_id, d_name FROM ref_delivery_service");
  _sql.append(" ORDER BY d_id ASC;");

  m_edit->clear();
  const QIcon _icon = AntiquaCRM::antiquaIcon("package");

  AntiquaCRM::ASqlCore t_sql(this);
  QSqlQuery _q = t_sql.query(_sql);
  if (_q.size() > 0) {
    int i = m_edit->count();
    while (_q.next()) {
      m_edit->insertItem(i, _icon, _q.value("d_name").toString());
      m_edit->setItemData(i, _q.value("d_id").toInt(), Qt::UserRole);
      i++;
    }
  }
}

void SelectDeliverService::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectDeliverService::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectDeliverService::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectDeliverService::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectDeliverService::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectDeliverService::getValue() {
  int _index = m_edit->currentIndex();
  return m_edit->itemData(_index, Qt::UserRole);
}

const QString SelectDeliverService::popUpHints() {
  return tr("a Deliver Service is required.");
}

const QString SelectDeliverService::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
