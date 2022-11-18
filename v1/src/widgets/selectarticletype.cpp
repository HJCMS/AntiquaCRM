// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectarticletype.h"

#include <QDebug>

SelectArticleType::SelectArticleType(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Article type"));
  m_layout->addWidget(m_box);
  loadDataset();
  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void SelectArticleType::loadDataset() {
  m_box->clear();
  /**< nicht festgelegt */
  m_box->insertItem(0, tr("Not set"), AntiquaCRM::ArticleType::UNKNOWN);
  /**< Bücher */
  m_box->insertItem(1, tr("Book"), AntiquaCRM::ArticleType::BOOK);
  /**< Film & Tonträger */
  m_box->insertItem(2, tr("Media"), AntiquaCRM::ArticleType::MEDIA);
  /**< Drucke & Stiche */
  m_box->insertItem(3, tr("Print"), AntiquaCRM::ArticleType::PRINTS);
  /**< Diverse */
  m_box->insertItem(4, tr("Other"), AntiquaCRM::ArticleType::OTHER);
}

void SelectArticleType::dataChanged(int) {
  setModified(true);
}

void SelectArticleType::reset() { setModified(false); }

void SelectArticleType::setValue(const QVariant &val) {
  int index = m_box->findData(val.toInt(), Qt::UserRole);
  if (index >= 0)
    m_box->setCurrentIndex(index);
}

void SelectArticleType::setFocus() { m_box->setFocus(); }

void SelectArticleType::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant SelectArticleType::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole);
}

bool SelectArticleType::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void SelectArticleType::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString SelectArticleType::info() { return m_box->toolTip(); }

const QString SelectArticleType::notes() {
  return tr("A valid media type is required.");
}
