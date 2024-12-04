// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sectionselecter.h"
#include "antiquaicon.h"

namespace AntiquaCRM
{

SectionSelecterModel::SectionSelecterModel(QWidget* parent)
    : QAbstractListModel{parent}, p_palette{parent->palette()} {
    beginInsertRows(QModelIndex(), 0, 5);
    endInsertRows();
}

QVariant SectionSelecterModel::typeName(ArticleType t) const {
  switch (t) {
    case (ArticleType::BOOK):
      return tr("Books");

    case (ArticleType::MEDIA):
      return tr("Media");

    case (ArticleType::PRINTS):
      return tr("Prints & Stitches");

    case (ArticleType::OTHER):
      return tr("Other");

    default:
      return tr("Unknown");
  }
}

int SectionSelecterModel::rowCount(const QModelIndex& ) const {
  return 5;
}

QVariant SectionSelecterModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return 0;

  int _r = index.row();
  if (_r > rowCount())
    return 0;

  ArticleType _t = static_cast<ArticleType>(index.row());
  switch (role) {
    case (Qt::DisplayRole):
      return typeName(_t);

    case (Qt::EditRole):
    case (Qt::UserRole):
      return _t;

    case (Qt::ToolTipRole):
      return tr("Tab");

    case (Qt::DecorationRole):
      return AntiquaCRM::antiquaIcon("action-tab");

    case (Qt::BackgroundRole):
      return ((_r % 2) & 1) ? p_palette.alternateBase() : p_palette.base();

    default:
      return QVariant();
  };

  return QVariant();
}

SectionSelecter::SectionSelecter(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  m_model = new SectionSelecterModel(m_edit);
  m_edit->setModel(m_model);
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SectionSelecter::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SectionSelecter::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setLength(3);
  setRestrictions(_f);
  setWindowModified(false);
}

void SectionSelecter::setValue(const QVariant& value) {
  int _set = value.toInt();
  if (_set > 0 && _set != m_edit->currentIndex())
    m_edit->setCurrentIndex(_set);
}

void SectionSelecter::setFocus() {
  m_edit->setFocus();
}

void SectionSelecter::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SectionSelecter::setRestrictions(const QSqlField& field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SectionSelecter::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void SectionSelecter::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SectionSelecter::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SectionSelecter::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SectionSelecter::getValue() {
  return m_edit->currentIndex();
}

const QString SectionSelecter::popUpHints() {
  return tr("Section tab selecter is required and must set.");
}

const QString SectionSelecter::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
