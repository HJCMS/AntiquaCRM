// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookbindingedit.h"

#include <AntiquaCRM>

namespace AntiquaCRM {

BookBindingEdit::BookBindingEdit(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  connect(m_edit, SIGNAL(currentIndexChanged(int)), this,
          SLOT(valueChanged(int)));
}

void BookBindingEdit::valueChanged(int index) {
  if (index == 0)
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void BookBindingEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(0);
  setRestrictions(_f);

  AntiquaCRM::ASharedDataFiles dataFiles;
  QJsonDocument doc = dataFiles.getJson("bookbindings");
  if (doc.isEmpty()) {
    qWarning("BookBindingEdit::bookbindings.json not found!");
    return;
  }

  m_edit->clear();
  m_edit->setWithoutDisclosures();
  QJsonArray arr = doc.object().value("bookbindings").toArray();
  for (int i = 0; i < arr.count(); i++) {
    QJsonObject obj = arr[i].toObject();
    m_edit->addItem(obj.value("description").toString(),
                    obj.value("id").toInt());
  }

  setWindowModified(false);
}

void BookBindingEdit::setValue(const QVariant &value) {
  int index = 0;
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
  case (QMetaType::Double): {
    index = m_edit->findData(value.toInt(), Qt::UserRole, Qt::MatchExactly);
  } break;

  default:
    qWarning("Invalid given Data Type in BookBindingEdit.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "BookBindingEdit Requires type int but get:" << value;
#endif
    break;
  };

  m_edit->setCurrentIndex(index);
}

void BookBindingEdit::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void BookBindingEdit::reset() {
  setWindowModified(false);
  m_edit->setCurrentIndex(0);
}

void BookBindingEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void BookBindingEdit::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void BookBindingEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool BookBindingEdit::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant BookBindingEdit::getValue() {
  int index = m_edit->currentIndex();
  return m_edit->itemData(index, Qt::UserRole).toInt();
}

const QString BookBindingEdit::popUpHints() {
  QStringList info;
  info << tr("A valid book binding is required for this input!");
  info << tr("Choose a book binding style for this item.");
  info << tr("Different types can set in the database configuration dialogs.");
  return info.join("\n");
}

const QString BookBindingEdit::statusHints() {
  return tr("Missing required book binding style!");
}

} // namespace AntiquaCRM
