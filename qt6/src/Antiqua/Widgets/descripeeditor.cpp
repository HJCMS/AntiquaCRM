// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "descripeeditor.h"
#include "splitter.h"

#include <QDebug>

namespace AntiquaCRM {

DescripeEditor::DescripeEditor(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  setObjectName("description_edit");

  m_splitter = new Splitter(this);
  layout->addWidget(m_splitter);
  layout->setStretchFactor(m_splitter, 1);

  m_edit = new AntiquaCRM::ATextEdit(this);
  m_splitter->addLeft(m_edit);
  m_splitter->setStretchFactor(0, 1);

  m_list = new QListWidget(this);
  m_list->setToolTip(tr("Predefined text blocks"));
  m_splitter->addRight(m_list);
  m_splitter->setStretchFactor(1, 0);

  initData();

  connect(m_edit, SIGNAL(textChanged()), SLOT(valueChanged()));
  connect(m_list, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          SLOT(appendText(QListWidgetItem *)));
}

void DescripeEditor::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
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

void DescripeEditor::setWordsList(const QStringList &list) {
  if (list.size() < 1)
    return;

  foreach (QString l, list) {
    m_list->addItem(l);
  }
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
  return tr("A valid Description is required.");
}

const QString DescripeEditor::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
