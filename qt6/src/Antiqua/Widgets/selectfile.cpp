// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectfile.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

namespace AntiquaCRM {

SelectFile::SelectFile(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setPlaceholderText(tr("set file path"));
  layout->addWidget(m_edit);

  m_open = new QPushButton(this);
  m_open->setIcon(AntiquaCRM::antiquaIcon("archive-insert-directory"));
  m_open->setToolTip(tr("Change file path"));
  layout->addWidget(m_open);

  initData();

  connect(m_open, SIGNAL(clicked()), SLOT(setFile()));
  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
}

bool SelectFile::isAccessible() {
  const QFileInfo _info(m_edit->text().trimmed());
  if (_info.isReadable()) {
    m_edit->setValidContent(true);
    return true;
  }
  m_edit->setValidContent(false);
  return false;
}

void SelectFile::valueChanged(const QString &data) {
  Q_UNUSED(data);
  if (isAccessible()) {
    setWindowModified(true);
    emit sendInputChanged();
    return;
  }
  setFocus();
}

void SelectFile::setFile() {
  QString _old = m_edit->text();
  QString _app = QFileDialog::getOpenFileName(this, m_edit->toolTip(), _old);
  QFileInfo _info(_app);
  if (_info.isWritable() && _info.filePath() != _old) {
    m_edit->setText(_info.filePath());
    valueChanged();
  }
}

void SelectFile::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
}

void SelectFile::setValue(const QVariant &value) {
  if (value.metaType().id() != getType().id())
    return;

  m_edit->setText(value.toString());
}

void SelectFile::setFocus() { m_edit->setFocus(); }

void SelectFile::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void SelectFile::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void SelectFile::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectFile::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectFile::isValid() {
  if (isRequired() && isAccessible())
    return true;

  return isAccessible();
}

const QMetaType SelectFile::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SelectFile::getValue() {
  return (isAccessible() ? m_edit->text().trimmed() : QString());
}

const QString SelectFile::popUpHints() {
  return tr("This entry requires a file path specification.");
}

const QString SelectFile::statusHints() {
  return tr("Required file or application path is missing!");
}

} // namespace AntiquaCRM
