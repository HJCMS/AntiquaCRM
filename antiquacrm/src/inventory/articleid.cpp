// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articleid.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

ArticleID::ArticleID(QWidget *parent) : QFrame{parent} {
  setObjectName("ArticleID");
  setWindowTitle(tr("Article ID"));
  setToolTip(tr("Article ID"));
  setContentsMargins(0, 0, 0, 0);
  setModified(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  QLabel *m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
  m_label->setText(tr("Article ID:"));
  layout->addWidget(m_label);

  id_label = new QLabel(this);
  id_label->setTextFormat(Qt::PlainText);
  id_label->setStyleSheet("font-weight: bold;");
  id_label->setAlignment(Qt::AlignLeft | Qt::AlignTrailing | Qt::AlignVCenter);
  id_label->setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                                    Qt::TextSelectableByMouse);
  layout->addWidget(id_label);
  setLayout(layout);
}

void ArticleID::setValue(const QVariant &id) {
  id_label->setText(id.toString());
  setModified(true);
}

void ArticleID::reset() {
  id_label->clear();
  setModified(false);
}

void ArticleID::setModified(bool b) { modified = b; }

void ArticleID::setRequired(bool b) { required = b; }

bool ArticleID::isRequired() { return required; }

bool ArticleID::hasModified() { return modified; }

const QString ArticleID::text() { return id_label->text(); }

const QVariant ArticleID::value() {
  QVariant data = QVariant(id_label->text()).toULongLong();
  return data;
}

bool ArticleID::isValid() {
  if (required && id_label->text().isEmpty())
    return false;

  if (QVariant(id_label->text()).toULongLong() < 1)
    return false;

  return true;
}

const QString ArticleID::notes() {
  return tr("The Arcticle ID is required and can not empty.");
}
