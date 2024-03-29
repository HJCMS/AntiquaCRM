// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdialog.h"
#include "configdatabase.h"
#include "configgeneral.h"
#include "configlookandfeel.h"
#include "configpageswidget.h"
#include "configpaths.h"
#include "configprinting.h"
#include "configprovidersview.h"
#include "configstoragecache.h"
#include "configtabsview.h"
#include "configtreewidget.h"

#include <AntiquaProviders>
#include <AntiquaTabs>

#include <QMessageBox>
#include <QMetaObject>
#include <QScrollArea>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("Configuration") + " [*]");
  setObjectName("configuration_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(780, 550);
  setContentsMargins(5, 5, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("antiqua_input_layout");
  layout->setContentsMargins(0, 0, 0, 0);

  AntiquaCRM::Splitter *m_splitter = new AntiquaCRM::Splitter(this);
  layout->addWidget(m_splitter);
  layout->setStretch(0, 1);

  QScrollArea *m_central = new QScrollArea(m_splitter);
  m_central->setWidgetResizable(true);
  m_splitter->addLeft(m_central);

  // Tree
  m_treeWidget = new ConfigTreeWidget(m_splitter);
  m_splitter->addRight(m_treeWidget);

  // Pages
  m_pageView = new ConfigPagesWidget(m_central);
  m_central->setWidget(m_pageView);

  int _pindex = m_pageView->count();
  m_cfgGeneral = new ConfigGeneral(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgGeneral->getMenuEntry());
  m_pageView->insert(_pindex++, m_cfgGeneral);

  m_cfgDatabase = new ConfigDatabase(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgDatabase->getMenuEntry());
  m_pageView->insert(_pindex++, m_cfgDatabase);

  m_cfgPaths = new ConfigPaths(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgPaths->getMenuEntry());
  m_pageView->insert(_pindex++, m_cfgPaths);

  m_cfgStorage = new ConfigStorageCache(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgStorage->getMenuEntry());
  m_pageView->insert(_pindex++, m_cfgStorage);

  m_cfgPrinter = new ConfigPrinting(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgPrinter->getMenuEntry());
  m_pageView->insert(_pindex++, m_cfgPrinter);

  // ConfigLookAndFeel
  m_cfgLookAndFeel = new ConfigLookAndFeel(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgLookAndFeel->getMenuEntry());
  m_pageView->insert(_pindex++, m_cfgLookAndFeel);

  // NOTE The Tabs interface section is a fixed TreeWidget entry!
  m_cfgTabs = new ConfigTabsView(m_pageView);
  m_pageView->insert(_pindex++, m_cfgTabs);

  // NOTE The Providers interface section is a fixed TreeWidget entry!
  m_cfgProviders = new ConfigProvidersView(m_pageView);
  m_pageView->insert(_pindex++, m_cfgProviders);

  m_buttonBox = new QDialogButtonBox(this);
  m_buttonBox->setOrientation(Qt::Horizontal);

  QPushButton *btn_save = m_buttonBox->addButton(QDialogButtonBox::Save);
  btn_save->setText(tr("&Save"));

  QPushButton *btn_cancel = m_buttonBox->addButton(QDialogButtonBox::Cancel);
  btn_cancel->setText(tr("Cancel"));

  QPushButton *btn_close = m_buttonBox->addButton(QDialogButtonBox::Close);
  btn_close->setText(tr("Quit"));

  layout->addWidget(m_buttonBox);

  m_statusbar = new QStatusBar(this);
  m_statusbar->setSizeGripEnabled(false);
  layout->addWidget(m_statusbar);

  setLayout(layout);

  connect(m_pageView, SIGNAL(sendModified(bool)), SLOT(setModified(bool)));
  connect(m_pageView, SIGNAL(sendPageTitle(const QString &)),
          SLOT(updateTitle(const QString &)));

  connect(m_treeWidget, SIGNAL(sendPageIndex(int)), m_pageView,
          SLOT(setPage(int)));

  connect(m_treeWidget, SIGNAL(sendConfigGroup(const QString &)),
          SLOT(openConfigGroup(const QString &)));

  connect(btn_save, SIGNAL(clicked()), SLOT(aboutToSave()));
  connect(btn_cancel, SIGNAL(clicked()), SLOT(reject()));
  connect(btn_close, SIGNAL(clicked()), SLOT(aboutToClose()));
}

ConfigDialog::~ConfigDialog() {}

void ConfigDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    if (isWindowModified()) {
      e->setAccepted(false);
      QString info = tr("You have unsaved changes!");
      info.append("<p>" + tr("Do your really want to close?") + "</p>");
      int ret = QMessageBox::question(this, tr("Unsaved Changes!"), info);
      if (ret == QMessageBox::No)
        return;
    }
  }
  QDialog::closeEvent(e);
}

bool ConfigDialog::loadConfigWidget() {
  AntiquaCRM::TabsLoader _pl_tabs(this);
  const QList<AntiquaCRM::TabsInterface *> _ti = _pl_tabs.interfaces(this);
  if (_ti.size() < 1)
    return false;

  int _count = m_pageView->count();
  QListIterator<AntiquaCRM::TabsInterface *> t_ti(_ti);
  while (t_ti.hasNext()) {
    AntiquaCRM::TabsInterface *m_iface = t_ti.next();
    if (m_iface) {
      AntiquaCRM::PluginConfigWidget *m_w = m_iface->configWidget(m_pageView);
      m_pageView->insert(_count, m_w);
      m_treeWidget->addTabPlugin(_count, m_w->getMenuEntry());
      _count++;
    }
  }

  m_pageView->insert(_count++, new ConfigProvidersView(m_pageView));

  AntiquaCRM::ProvidersLoader _pl_providers(this);
  const QList<AntiquaCRM::ProviderInterface *> _pi =
      _pl_providers.interfaces(this);
  if (_pi.size() < 1)
    return false;

  QListIterator<AntiquaCRM::ProviderInterface *> t_pi(_pi);
  while (t_pi.hasNext()) {
    AntiquaCRM::ProviderInterface *m_iface = t_pi.next();
    if (m_iface) {
      AntiquaCRM::PluginConfigWidget *m_w = m_iface->configWidget(m_pageView);
      m_pageView->insert(_count, m_w);
      m_treeWidget->addProviderPlugin(_count, m_w->getMenuEntry());
      _count++;
    }
  }

  return true;
}

void ConfigDialog::loadConfigs() {
  QListIterator<AntiquaCRM::PluginConfigWidget *> it(m_pageView->pages());
  while (it.hasNext()) {
    it.next()->loadSectionConfig();
  }
}

void ConfigDialog::setModified(bool b) {
  setWindowModified(b);
  // qDebug() << Q_FUNC_INFO << isWindowModified();
}

void ConfigDialog::updateTitle(const QString &title) {
  QString _title(tr("Configuration"));
  _title.append(" (");
  _title.append(title);
  _title.append(") [*]");
  setWindowTitle(_title);
}

void ConfigDialog::statusMessage(const QString &message) {
  m_statusbar->showMessage(message, 5000);
}

void ConfigDialog::openConfigGroup(const QString &name) {
  if (!name.startsWith("config_"))
    return;

  for (int i = 0; i < m_pageView->count(); i++) {
    if (m_pageView->widget(i)->objectName() == name) {
      m_pageView->setPage(i);
      return;
    }
  }
}

void ConfigDialog::aboutToSave() {
  QListIterator<AntiquaCRM::PluginConfigWidget *> it(m_pageView->pages());
  while (it.hasNext()) {
    it.next()->saveSectionConfig();
  }
}

void ConfigDialog::aboutToClose() {
  if (isWindowModified()) {
    m_statusbar->showMessage(tr("unsaved changes!"));
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

  if (!loadConfigWidget())
    qWarning("Can't load Configuration plugins!");

  loadConfigs();
  setWindowModified(false);
  return QDialog::exec();
}
