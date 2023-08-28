// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectdeliverservice.h"
#include "antiquaicon.h"

namespace AntiquaCRM {

SelectDeliverService::SelectDeliverService(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setMinimumContentsLength(8);
  layout->addWidget(m_edit);

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectDeliverService::valueChanged(int index) {
  setWindowModified(true);
  emit sendInputChanged();
  if (index >= 0)
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
  m_edit->clear();
  const QIcon _icon = AntiquaCRM::qrcIcon("package-deliver");
  const QString _union_query =
      AntiquaCRM::ASqlFiles::queryStatement("union_default_delivery");
  if (_union_query.isEmpty())
    return;

  AntiquaCRM::ASqlCore t_sql(this);
  QSqlQuery _q = t_sql.query(_union_query);
  if (_q.size() > 0) {
    int _index = 0; // without disclosure
    int _count = m_edit->count();
    while (_q.next()) {
      // d_id, d_name, d_index
      int d_id = _q.value("d_id").toInt();
      const QString d_name = _q.value("d_name").toString();
      // add only items with a package name
      if (d_name.isEmpty()) {
        // if d_index equal to d_id then mark as current index
        // NOTE: Default index - don't have a title or name!
        if (d_id == _q.value("d_index").toInt())
          _index = _count;

        continue;
      }
      m_edit->insertItem(_count, _icon, d_name);
      m_edit->setItemData(_count, d_id, Qt::UserRole);
      _count++;
    }
    if (_index > 0)
      m_edit->setCurrentIndex(_index);

    _q.clear();
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
