// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdialog.h"
#include "configgeneral.h"

#include <QMessageBox>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("Configuration") + " [*]");
  setSizeGripEnabled(true);
  setMinimumSize(780, 550);
  setContentsMargins(5, 5, 5, 0);

  layout = new QVBoxLayout(this);
  layout->setObjectName("antiqua_input_layout");
  layout->setContentsMargins(0, 0, 0, 0);

  m_pageView = new QTabWidget(this);
  layout->addWidget(m_pageView);
  layout->setStretch(0, 1);

  m_cfgGeneral = new ConfigGeneral(m_pageView);
  m_pageView->insertTab(0, m_cfgGeneral, m_cfgGeneral->getIcon(),
                        m_cfgGeneral->getTitle());

  m_buttonBox = new QDialogButtonBox(this);
  m_buttonBox->setOrientation(Qt::Horizontal);

  QPushButton *btn_save = m_buttonBox->addButton(QDialogButtonBox::Save);
  btn_save->setText(tr("&Save"));

  QPushButton *btn_close = m_buttonBox->addButton(QDialogButtonBox::Close);
  btn_close->setText(tr("Quit"));

  layout->addWidget(m_buttonBox);

  m_statusbar = new QStatusBar(this);
  m_statusbar->setSizeGripEnabled(false);
  layout->addWidget(m_statusbar);

  setLayout(layout);

  connect(btn_save, SIGNAL(clicked()), this, SLOT(aboutToSave()));
  connect(btn_close, SIGNAL(clicked()), this, SLOT(aboutToClose()));
}

ConfigDialog::~ConfigDialog() {}

const QList<AntiquaCRM::TabsConfigWidget *> ConfigDialog::groups() {
  return m_pageView->findChildren<AntiquaCRM::TabsConfigWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

void ConfigDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    if (isWindowModified()) {
      e->setAccepted(false);
      QString info = tr("You have unsafed changes!");
      info.append("<p>" + tr("Do your really want to close?") + "</p>");
      int ret = QMessageBox::question(this, tr("Unsaved Changes!"), info);
      if (ret == QMessageBox::No)
        return;
    }
  }
  QDialog::closeEvent(e);
}

void ConfigDialog::loadConfigs() {
  QListIterator<AntiquaCRM::TabsConfigWidget *> it(groups());
  while (it.hasNext()) {
    it.next()->loadSectionConfig();
  }
}

void ConfigDialog::statusMessage(const QString &message) {
  m_statusbar->showMessage(message, 5000);
}

void ConfigDialog::aboutToSave() {}

void ConfigDialog::aboutToClose() {
  if (isWindowModified()) {
    m_statusbar->showMessage(tr("Unsafed changes!"));
    return;
  }

  config->beginGroup("dialog/configuration");
  config->setValue("geometry", saveGeometry());
  config->endGroup();

  accept();
}

int ConfigDialog::exec() {
  config = new AntiquaCRM::ASettings(this);
  config->beginGroup("dialog/configuration");
  config->value("geometry", saveGeometry());
  if (config->contains("geometry"))
    restoreGeometry(config->value("geometry").toByteArray());
  config->endGroup();

  loadConfigs();
  setWindowModified(false);
  return QDialog::exec();
}
