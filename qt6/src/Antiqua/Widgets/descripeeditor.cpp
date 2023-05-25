// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "descripeeditor.h"

#include <QDebug>

namespace AntiquaCRM {

DescripeEditor::DescripeEditor(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ATextEdit(this);
  layout->addWidget(m_edit);

  m_list = new QListWidget(this);
  m_list->setToolTip(tr("Predefined text blocks"));
  layout->addWidget(m_list);

  layout->setStretchFactor(m_edit, 1);
  layout->setStretchFactor(m_list, 0);

  initData();

  connect(m_edit, SIGNAL(textChanged()), SLOT(valueChanged()));
  connect(m_list, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          SLOT(appendText(QListWidgetItem *)));
}

void DescripeEditor::valueChanged() {
  setWindowModified(true);
  emit sendInputChanged();
}

void DescripeEditor::appendText(QListWidgetItem *item) {
  const QString _txt = item->text();
  m_edit->setFocus();
  m_edit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  if (m_edit->find(_txt, QTextDocument::FindBackward)) {
    return;
  }

  QString _buf;
  if (m_edit->text().length() > 0) {
    _buf = m_edit->text().trimmed();
    _buf.append(", ");
    _buf.append(_txt);
  } else {
    _buf.append(_txt);
  }
  m_edit->clear();
  m_edit->setText(_buf.trimmed());
  _buf.clear();
}

void DescripeEditor::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);

  QStringList _list;
  _list << tr("Nice clean copy");
  _list << tr("Clean copy");
  _list << tr("Copy a bit dusty");
  _list << tr("Year may differ from entry");
  _list << tr("Age-related traces");
  _list << tr("With bookplate");

  foreach (QString l, _list) {
    m_list->addItem(l);
  }
}

void DescripeEditor::setValue(const QVariant &value) {
  if (value.metaType().id() == QMetaType::QString) {
    m_edit->setText(value.toString());
  } else {
    qWarning("Invalid value add to DescripeEditor");
  }
}

void DescripeEditor::setFocus() { m_edit->setFocus(); }

void DescripeEditor::reset() {
  m_edit->setText(QString());
  m_edit->clear();
  setWindowModified(false);
}

void DescripeEditor::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void DescripeEditor::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void DescripeEditor::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool DescripeEditor::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QMetaType DescripeEditor::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant DescripeEditor::getValue() { return m_edit->text(); }

const QString DescripeEditor::popUpHints() {
  return tr("This Description field requires a valid input.");
}

const QString DescripeEditor::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
