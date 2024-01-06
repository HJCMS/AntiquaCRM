// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectmediatype.h"

namespace AntiquaCRM {

SelectMediaType::SelectMediaType(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  QIcon _warn = style()->standardIcon(QStyle::SP_MessageBoxWarning);
  QIcon _icon = style()->standardIcon(QStyle::SP_DriveCDIcon);
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->addItem(_warn, m_edit->withoutDisclosures(), // no media
                  AntiquaCRM::MEDIA_NO_TYPE);
  m_edit->addItem(_icon, tr("Compact Disc"), // disc
                  AntiquaCRM::MEDIA_DISC_COMPACT);
  m_edit->addItem(_icon, tr("Mini Compact Disc"), // disc
                  AntiquaCRM::MEDIA_DISC_MINI);
  m_edit->addItem(_icon, tr("Video Compact Disc"), // video disc
                  AntiquaCRM::MEDIA_DISC_VIDEO);
  m_edit->addItem(_icon, tr("Vinyl single record"), // vinyl
                  AntiquaCRM::MEDIA_VINYL_SINGLE);
  m_edit->addItem(_icon, tr("Vinyl longplay record"),
                  AntiquaCRM::MEDIA_VINYL_LONGPLAY);
  m_edit->addItem(_icon, tr("Vinyl Multi Groove"),
                  AntiquaCRM::MEDIA_VINYL_MULTI_GROOVE);
  m_edit->addItem(_icon, tr("Vinyl colored record"), // vinyl
                  AntiquaCRM::MEDIA_VINYL_COLORED);
  m_edit->addItem(_icon, tr("Vinyl video record"), // vinyl
                  AntiquaCRM::MEDIA_VINYL_VIDEO);
  m_edit->addItem(_icon, tr("Vinyl record and DISC"), // vinyl
                  AntiquaCRM::MEDIA_VINYL_DISC);
  layout->addWidget(m_edit);

  initData();

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectMediaType::valueChanged(int) { setWindowModified(true); }

void SelectMediaType::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(AntiquaCRM::MEDIA_NO_TYPE);
  setRestrictions(_f);
  setWindowModified(false);
}

void SelectMediaType::setValue(const QVariant &value) {
  if (value.metaType().id() != getType().id())
    return;

  int _index = m_edit->findData(value.toInt(), Qt::UserRole, Qt::MatchExactly);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SelectMediaType::setFocus() { m_edit->setFocus(); }

void SelectMediaType::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectMediaType::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void SelectMediaType::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectMediaType::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectMediaType::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectMediaType::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectMediaType::getValue() {
  int _index = m_edit->currentIndex();
  return m_edit->itemData(_index, Qt::UserRole);
}

const QString SelectMediaType::popUpHints() {
  return tr("Please select a valid Media type for this Input.");
}

const QString SelectMediaType::statusHints() {
  return tr("A valid Media type is required!");
}

} // namespace AntiquaCRM
