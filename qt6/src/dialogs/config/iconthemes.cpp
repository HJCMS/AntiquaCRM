// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "iconthemes.h"

#include <QtCore>

IconThemes::IconThemes(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent}, QIcon{} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setMinimumWidth(200);
  layout->addWidget(m_edit);
  initData();

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

const QString IconThemes::getName(const QString &path) const {
  QString _out;
  const QFileInfo _index(path, "index.theme");
  if (!_index.isReadable())
    return _out;

  // only cachable desktop theme
  const QFileInfo _cache(path, "icon-theme.cache");
  if (!_cache.isReadable())
    return _out;

  QString _lcName("Name[");
  _lcName.append(QLocale::system().bcp47Name());
  _lcName.append("]");

  QSettings _tcfg(_index.filePath(), QSettings::IniFormat);
  _tcfg.beginGroup("Icon Theme");
  const QString _key(_tcfg.contains(_lcName) ? _lcName : "Name");
  _out = _tcfg.value(_key).toString();
  _tcfg.endGroup();
  return _out;
}

void IconThemes::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void IconThemes::setValue(const QVariant &value) {
  int _index = m_edit->findData(value, Qt::DisplayRole);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void IconThemes::setFocus() { m_edit->setFocus(); }

void IconThemes::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void IconThemes::initData() {
  const QIcon _iIcon = AntiquaCRM::antiquaIcon("preferences-desktop");
  QStringList _buffer("hicolor");
  QDir::Filters _filter = (QDir::Dirs | QDir::NoDotAndDotDot);
  int _index = 0;
  foreach (QString p, QIcon::themeSearchPaths()) {
    QDir _dir(p);
    foreach (QString _dn, _dir.entryList(_filter, QDir::Name)) {
      _dir.cd(_dn);
      const QString _name = getName(_dir.path());
      const QString _dirname = _dn.toLower();
      if (!_name.isEmpty() && !_buffer.contains(_dirname)) {
        m_edit->insertItem(_index, _iIcon, _name, _dn);
        m_edit->setItemData(_index, _name, Qt::ToolTipRole);
        _buffer.append(_dirname);

        if (_name.toLower() == themeName().toLower())
          m_edit->setCurrentIndex(_index);

        _index++;
      }
      _dir.cdUp();
    }
  }
  _buffer.clear();
}

void IconThemes::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void IconThemes::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void IconThemes::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  AntiquaCRM::ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool IconThemes::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType IconThemes::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant IconThemes::getValue() {
  return m_edit->itemData(m_edit->currentIndex(), Qt::UserRole);
}

const QString IconThemes::popUpHints() {
  return tr("Icon Theme name is missing.");
}

const QString IconThemes::statusHints() { return popUpHints(); }
