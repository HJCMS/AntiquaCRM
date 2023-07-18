// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectdeliverpackage.h"

namespace AntiquaCRM {

SelectDeliverPackage::SelectDeliverPackage(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_sql = new AntiquaCRM::ASqlCore(this);

  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);

  m_price = new ALabel(this);
  layout->addWidget(m_price);

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

SelectDeliverPackage::~SelectDeliverPackage() {
  if (m_sql != nullptr)
    m_sql->deleteLater();
}

void SelectDeliverPackage::valueChanged(int) {
  m_price->setText(ATaxCalculator::money(getPackagePrice()));
  setWindowModified(true);
  emit sendInputChanged();
}

void SelectDeliverPackage::loadPackages(int service) {
  if (service < 1)
    return;

  int _default = -1;
  m_edit->clear();
  QString _sql("SELECT d_cid,d_class,d_definition,d_default");
  _sql.append(" FROM ref_delivery_cost WHERE d_srv=");
  _sql.append(QString::number(service));
  _sql.append(" ORDER BY d_international ASC;");
  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() > 0) {
    int i = 0;
    while (_q.next()) {
      QString txt = _q.value("d_class").toString();
      txt.append(" ");
      txt.append(_q.value("d_definition").toString());
      m_edit->insertItem(i, txt);
      m_edit->setItemData(i, _q.value("d_cid").toInt(), Qt::UserRole);

      if (_q.value("d_default").toBool())
        _default = i;

      i++;
    }
  }
  if (_default >= 0) {
    m_edit->setCurrentIndex(_default);
  }
}

void SelectDeliverPackage::setValue(const QVariant &value) {
  if (m_edit->count() < 1)
    return;

  if (value.metaType().id() != getType().id()) {
    qWarning("Invalid Delivery package number!");
    return;
  }

  int _index = m_edit->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (_index >= 0) {
    m_edit->setCurrentIndex(_index);
    m_price->setText(ATaxCalculator::money(getPackagePrice()));
  }
}

void SelectDeliverPackage::setFocus() { m_edit->setFocus(); }

void SelectDeliverPackage::reset() {
  m_edit->setCurrentIndex(0);
  m_price->clear();
  setWindowModified(false);
}

void SelectDeliverPackage::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectDeliverPackage::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectDeliverPackage::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectDeliverPackage::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectDeliverPackage::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectDeliverPackage::getValue() {
  int _index = m_edit->currentIndex();
  return m_edit->itemData(_index, Qt::UserRole);
}

const QString SelectDeliverPackage::popUpHints() {
  return tr("a Deliver Package is required.");
}

const QString SelectDeliverPackage::statusHints() { return popUpHints(); }

qreal SelectDeliverPackage::getPackagePrice() {
  int _id = getValue().toInt();
  if (_id < 0)
    return 0.00;

  QString _sql("SELECT d_price FROM ref_delivery_cost WHERE d_cid=");
  _sql.append(QString::number(_id) + ";");
  QSqlQuery q = m_sql->query(_sql);
  if (q.next()) {
    return q.value("d_price").toDouble();
  }
  return 0.00;
}

bool SelectDeliverPackage::isInternational() {
  int _id = getValue().toInt();
  if (_id < 0)
    return false;

  QString _sql("SELECT d_international FROM ref_delivery_cost WHERE d_cid=");
  _sql.append(QString::number(_id) + ";");
  QSqlQuery q = m_sql->query(_sql);
  if (q.next()) {
    return q.value("d_international").toBool();
  }
  return false;
}

} // namespace AntiquaCRM