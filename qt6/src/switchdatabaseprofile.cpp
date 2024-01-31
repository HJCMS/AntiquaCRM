// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "switchdatabaseprofile.h"

#include <AntiquaWidgets>
#include <QLabel>
#include <QLayout>

SwitchDatabaseProfile::SwitchDatabaseProfile(AntiquaCRM::ASettings *settings,
                                             QWidget *parent)
    : QDialog{parent}, config{settings} {
  setObjectName("switch_db_connection");
  setWindowTitle(tr("Database connection"));
  setMinimumSize(450, 400);
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QString _body("<p>");
  _body.append(tr("Either there is no Internet connection at the target level "
                  "or the DynDns registration isn't available."));
  _body.append("</p><p>");
  _body.append(tr("You can wait until the server is available again or contact "
                  "the system administrator."));
  _body.append("</p><p>");
  _body.append(tr("If you have configured more than one connection profile, "
                  "you can alternatively switch."));
  _body.append("</p>");

  QStringList _info;
  _info << "<b>" + tr("SQL Server is temporarily unavailable.") + "</b>";
  _info << _body;

  QLabel *m_info = new QLabel(this);
  m_info->setText(_info.join(""));
  m_info->setWordWrap(true);
  layout->addWidget(m_info);

  m_sqlError = new QTextEdit(this);
  layout->addWidget(m_sqlError);

  layout->addStretch();

  sql_profiles = new QComboBox(this);
  sql_profiles->setObjectName("database_profile");
  sql_profiles->setToolTip(tr("Database profiles"));
  sql_profiles->addItem(tr("Database profile"), QString());
  layout->addWidget(sql_profiles, 0, Qt::AlignLeft);

  btn_box = new QDialogButtonBox(
      (QDialogButtonBox::Save | QDialogButtonBox::Cancel), this);
  layout->addWidget(btn_box);

  setLayout(layout);

  connect(btn_box, SIGNAL(accepted()), SLOT(saveAndQuit()));
  connect(btn_box, SIGNAL(rejected()), SLOT(reject()));
}

void SwitchDatabaseProfile::saveAndQuit() {
  const QString _profile = sql_profiles->currentData().toString();
  if (_profile.isEmpty())
    return; // no selection

  const QString _old = config->value("database_profile", "Default").toString();
  if (_profile == _old) {
    accept();
    return; // nothing todo
  }

  qInfo("Changed profile '%s' and close dialog.", qPrintable(_profile));
  config->setValue("database_profile", _profile);
  accept();
}

void SwitchDatabaseProfile::setRemoteInfo(const QString &msg) {
  m_sqlError->setPlainText(msg);
}

int SwitchDatabaseProfile::exec() {
  const QIcon _icon = AntiquaCRM::antiquaIcon("server-database");
  config->beginGroup("database");
  foreach (QString _group, config->childGroups()) {
    QString _title = config->value(_group + "/profile").toString();
    if (_title.isEmpty())
      continue;

    sql_profiles->addItem(_icon, _title, _group);
  }
  config->endGroup();

  QString _index = config->value("database_profile", "Default").toString();
  for (int i = 0; i < sql_profiles->count(); i++) {
    QString _data = sql_profiles->itemData(i, Qt::UserRole).toString();
    if (_data == _index)
      sql_profiles->setCurrentIndex(i);
  }

  return QDialog::exec();
}
