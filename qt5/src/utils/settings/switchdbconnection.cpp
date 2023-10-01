// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "switchdbconnection.h"

#include <QLabel>
#include <QLayout>

SwitchDBConnection::SwitchDBConnection(QWidget *parent) : QDialog{parent} {
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

  layout->addStretch();

  sql_profiles = new QComboBox(this);
  sql_profiles->setObjectName("database_profile");
  sql_profiles->setToolTip(tr("Database profile"));
  sql_profiles->addItem(tr("Database profile"), QString());
  layout->addWidget(sql_profiles, 0, Qt::AlignLeft);

  btn_box = new QDialogButtonBox(
      (QDialogButtonBox::Save | QDialogButtonBox::Cancel), this);
  layout->addWidget(btn_box);

  setLayout(layout);

  connect(btn_box, SIGNAL(accepted()), SLOT(saveAndQuit()));
  connect(btn_box, SIGNAL(rejected()), SLOT(reject()));
}

void SwitchDBConnection::saveAndQuit() {
  QString _profile = sql_profiles->currentData().toString();
  if (_profile.isEmpty())
    return;

  cfg->setValue("database_profile", _profile);
  accept();
}

int SwitchDBConnection::exec() {
  cfg = new AntiquaCRM::ASettings(this);
  cfg->beginGroup("database");
  foreach (QString _g, cfg->childGroups()) {
    if (_g.isEmpty())
      continue;

    sql_profiles->addItem(_g, _g);
  }
  cfg->endGroup();

  QString _index = cfg->value("database_profile", "Default").toString();
  sql_profiles->setCurrentText(_index);

  return QDialog::exec();
}
