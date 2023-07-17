// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "databaseprofile.h"

#include <AntiquaInput>
#include <QIcon>
#include <QInputDialog>
#include <QLayout>
#include <QMessageBox>

DatabaseProfile::DatabaseProfile(AntiquaCRM::ASettings *config, QWidget *parent)
    : QFrame{parent}, cfg{config}, p_profil{"Default"} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(2, 0, 2, 0);

  m_select = new AntiquaCRM::AComboBox(this);
  m_select->setObjectName("database_profile");
  m_select->setWithoutDisclosures("default");
  m_select->setMinimumWidth(200);
  m_select->setToolTip(tr("Database profile"));
  layout->addWidget(m_select);

  layout->addStretch(1);

  m_test = new QPushButton(tr("Test"), this);
  m_test->setToolTip(tr("Create connection test."));
  m_test->setIcon(AntiquaCRM::antiquaIcon("debug-run"));
  layout->addWidget(m_test);

  m_create = new QPushButton(tr("Create"), this);
  m_create->setToolTip(tr("Create a new Profile entry."));
  m_create->setIcon(AntiquaCRM::antiquaIcon("action-add"));
  layout->addWidget(m_create);

  m_remove = new QPushButton(tr("Remove"), this);
  m_remove->setToolTip(tr("Remove Selected Profile."));
  m_remove->setIcon(AntiquaCRM::antiquaIcon("action-remove"));
  m_remove->setEnabled(false);
  layout->addWidget(m_remove);

  setLayout(layout);

  connect(m_test, SIGNAL(clicked()), SIGNAL(sendStartTest()));
  connect(m_create, SIGNAL(clicked()), SLOT(actionCreate()));
  connect(m_remove, SIGNAL(clicked()), SLOT(actionRemove()));
}

const QIcon DatabaseProfile::entryIcon() {
  return AntiquaCRM::antiquaIcon("server-database");
}

void DatabaseProfile::actionCreate() {
  bool _accept = false;
  const QString _title = tr("Create a new Profile entry.");
  const QString _label = tr("Enter the new profile title here:");
  QString _ret =
      QInputDialog::getText(this, _title, _label, QLineEdit::Normal, QString(),
                            &_accept, Qt::Tool, Qt::ImhSensitiveData);

  if (_accept && !_ret.isEmpty()) {
    QString _title = _ret.trimmed();
    QString _key = AntiquaCRM::AUtil::md5sum(_title);
    if (m_select->findData(_key, Qt::UserRole, Qt::MatchExactly) == -1) {
      int _index = m_select->count();
      m_select->insertItem(_index, entryIcon(), _title, _key);
      m_select->setCurrentIndex(_index);
      emit sendUpdateProfile();
    }
    m_remove->setEnabled((m_select->count() > 2));
  }
}

void DatabaseProfile::actionRemove() {
  if (m_select->count() < 1)
    return;

  int _index = m_select->currentIndex();
  if (_index < 1)
    return;

  const QString _title = tr("Delete profile");
  QString _question = tr("You really want to delete the '%1' Profile?")
                          .arg(m_select->currentText());

  const QString _key = m_select->itemData(_index, Qt::UserRole).toString();
  if (_key == p_profil) {
    _question = tr("This Profile is protected and can not deleted!");
    QMessageBox::warning(this, _title, _question);
    return;
  }

  if (QMessageBox::question(this, _title, _question) == QMessageBox::Yes) {
    cfg->beginGroup("database");
    foreach (QString group, cfg->childGroups()) {
      if (group == _key)
        cfg->remove(group);
    }
    cfg->endGroup();
    m_select->removeItem(_index);
    emit sendUpdateProfile();
  }
}

void DatabaseProfile::setProfile(int index) {
  QString _id = m_select->itemData(index, Qt::UserRole).toString();
  if (_id.isEmpty())
    return;

  emit sendUpdateProfile();
}

void DatabaseProfile::findProfile(const QString &name) {
  for (int i = 0; i < m_select->count(); i++) {
    QString _data = m_select->itemData(i, Qt::UserRole).toString();
    if (_data == name)
      m_select->setCurrentIndex(i);
  }
}

void DatabaseProfile::loadEntries() {
  cfg->beginGroup("database");
  foreach (QString _group, cfg->childGroups()) {
    QString _title = cfg->value(_group + "/profile").toString();
    if (_title.isEmpty())
      continue;

    m_select->addItem(entryIcon(), _title, _group);
  }
  cfg->endGroup();
  findProfile(cfg->value("database_profile", p_profil).toString());
  m_remove->setEnabled((m_select->count() > 2));
  // prevent multible currentIndexChanged(int) onload
  connect(m_select, SIGNAL(currentIndexChanged(int)), SLOT(setProfile(int)));
}

const QString DatabaseProfile::currentProfile() {
  return m_select->currentText();
}

const QString DatabaseProfile::currentIdentifier() {
  int _index = m_select->currentIndex();
  if (_index > 0) {
    return m_select->itemData(_index, Qt::UserRole).toString();
  }
  return p_profil;
}
