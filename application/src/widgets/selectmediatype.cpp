// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectmediatype.h"

#include <QDebug>

SelectMediaType::SelectMediaType(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Mediatype"));
  m_layout->addWidget(m_box);
  loadDataset();
  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void SelectMediaType::loadDataset() {
  QMapIterator<AntiquaCRM::MediaType, QString> it(list());
  while (it.hasNext()) {
    it.next();
    m_box->addItem(it.value(), it.key());
  }
}

void SelectMediaType::dataChanged(int) { setModified(true); }

void SelectMediaType::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void SelectMediaType::setValue(const QVariant &val) {
  int v = val.toInt();
  if (v < 1)
    return;

  for (int i = 0; i < m_box->count(); i++) {
    int d = m_box->itemData(i, Qt::UserRole).toInt();
    if (d == v) {
      m_box->setCurrentIndex(i);
      break;
    }
  }
}

void SelectMediaType::setFocus() {
  m_box->showPopup();
  m_box->setFocus();
}

void SelectMediaType::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant SelectMediaType::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole);
}

bool SelectMediaType::isValid() {
  if (isRequired() && value().toInt() == 0)
    return false;

  return true;
}

void SelectMediaType::setInfo(const QString &info) { setToolTip(info); }

const QString SelectMediaType::info() { return m_box->toolTip(); }

const QString SelectMediaType::notes() {
  return tr("A valid Mediatype is required.");
}

const QMap<AntiquaCRM::MediaType, QString> SelectMediaType::list() {
  QString very_rare(" (" + tr("very rare") + ")");
  QMap<AntiquaCRM::MediaType, QString> m;
  m.insert(AntiquaCRM::MediaType::MEDIA_NO_TYPE, tr("no Mediatype selected"));
  m.insert(AntiquaCRM::MediaType::MEDIA_DISC_COMPACT,
           tr("Compact Disc") + " (CD)");
  m.insert(AntiquaCRM::MediaType::MEDIA_DISC_MINI,
           tr("Mini Disc") + " (Mini-CD)");
  m.insert(AntiquaCRM::MediaType::MEDIA_DISC_VIDEO,
           tr("CD Video") + " (CD-V)"); /**< @brief CDV */
  m.insert(AntiquaCRM::MediaType::MEDIA_VINYL_SINGLE,
           tr("Single Vinyl")); /**< @brief Single */
  m.insert(AntiquaCRM::MediaType::MEDIA_VINYL_LONGPLAY,
           tr("Longplay vinyl record") + " (LP)");
  m.insert(AntiquaCRM::MediaType::MEDIA_VINYL_MULTI_GROOVE,
           tr("Multi groove vinyl record") + very_rare);
  m.insert(AntiquaCRM::MediaType::MEDIA_VINYL_COLORED,
           tr("Colored Longplay vinyl record") + " (LP)");
  m.insert(AntiquaCRM::MediaType::MEDIA_VINYL_VIDEO,
           tr("Videoplay vinyl record") + very_rare);
  m.insert(AntiquaCRM::MediaType::MEDIA_VINYL_DISC,
           tr("Vinyl and Compakt Disc") + very_rare);
  return m;
}
