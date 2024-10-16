// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selecttargets.h"
#include "antiquaicon.h"

#ifdef Q_OS_LINUX
#  include <fcntl.h>
#  include <unistd.h>
#  define _SYSTEM_ACCESS_MODES (F_OK | X_OK | W_OK | R_OK)
#endif

#ifdef Q_OS_WIN
#  include <io.h>
#  define _SYSTEM_ACCESS_MODES (00 | 01 | 02 | 04)
#endif

#include <QByteArray>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QUrl>

namespace AntiquaCRM
{

SelectTargets::SelectTargets(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setPlaceholderText(tr("Target directory"));
  layout->addWidget(m_edit);

  m_open = new QPushButton(this);
  m_open->setIcon(AntiquaCRM::antiquaIcon("archive-insert-directory"));
  m_open->setToolTip(tr("Change target directory"));
  layout->addWidget(m_open);

  connect(m_open, SIGNAL(clicked()), SLOT(setTarget()));

  initData();
}

bool SelectTargets::isAccessible() {
  bool _ret = false;
  const QDir _dir(m_edit->text().trimmed());
  if (_dir.exists()) {
#ifdef _SYSTEM_ACCESS_MODES
    QByteArray _p = _dir.path().toUtf8();
    // If path cannot be found or if any of the desired access modes would not
    // be granted, then a -1 value is returned.
    _ret = (access(_p.constData(), _SYSTEM_ACCESS_MODES) != -1);
#else
    if (_dir.mkdir("antiquacrm_accessible_test")) {
      _ret = _dir.rmdir("antiquacrm_accessible_test");
    }
#endif
  }

  if (_ret) {
    m_edit->setValidContent(true);
    return true;
  }
  m_edit->setValidContent(false);
  return false;
}

void SelectTargets::valueChanged(const QString& data) {
  Q_UNUSED(data);
  if (isAccessible()) {
    setWindowModified(true);
    emit sendInputChanged();
    return;
  }
  setFocus();
}

void SelectTargets::setTarget() {
  QString _old = m_edit->text();
  QString _dir = QFileDialog::getExistingDirectory(this, m_edit->toolTip(), _old);
  QFileInfo _info(_dir);
  if (_info.isWritable() && _info.filePath() != _old) {
    m_edit->setText(_info.filePath());
    valueChanged();
  }
}

void SelectTargets::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  setRestrictions(_f);
  setWindowModified(false);
}

void SelectTargets::setValue(const QVariant& value) {
  if (value.metaType().id() != getType().id())
    return;

  QString _str = value.toString().trimmed();
  if (_str.isEmpty())
    return;

  m_edit->setText(AUtil::pathDecoded(_str));
}

void SelectTargets::setFocus() {
  m_edit->setFocus();
}

void SelectTargets::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void SelectTargets::setRestrictions(const QSqlField& field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (m_edit->text().trimmed().isEmpty()) {
    m_edit->setText(field.defaultValue().toString());
  }
}

void SelectTargets::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void SelectTargets::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectTargets::isValid() {
  if (isRequired() && isAccessible())
    return true;

  return isAccessible();
}

const QMetaType SelectTargets::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SelectTargets::getValue() {
  if (!isAccessible())
    return QString();

  return m_edit->text().trimmed();
}

const QString SelectTargets::popUpHints() {
  return tr("This entry requires a directory specification.");
}

const QString SelectTargets::statusHints() {
  return tr("Required directory is missing!");
}

} // namespace AntiquaCRM
