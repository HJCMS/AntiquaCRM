// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdialog.h"
#include "configgeneral.h"
#include "configlookandfeel.h"
#include "configpaths.h"
#include "configprinting.h"
#include "configprovidersview.h"
#include "configtabsview.h"
#include "configtreewidget.h"

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

  m_pageView = new QStackedWidget(m_central);
  m_central->setWidget(m_pageView);

  int _pindex = m_pageView->count();
  m_cfgGeneral = new ConfigGeneral(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgGeneral->getMenuEntry());
  m_pageView->insertWidget(_pindex++, m_cfgGeneral);

  m_cfgPaths = new ConfigPaths(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgPaths->getMenuEntry());
  m_pageView->insertWidget(_pindex++, m_cfgPaths);

  m_cfgPrinter = new ConfigPrinting(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgPrinter->getMenuEntry());
  m_pageView->insertWidget(_pindex++, m_cfgPrinter);

  // ConfigLookAndFeel
  m_cfgLookAndFeel = new ConfigLookAndFeel(m_pageView);
  m_treeWidget->addGeneral(_pindex, m_cfgLookAndFeel->getMenuEntry());
  m_pageView->insertWidget(_pindex++, m_cfgLookAndFeel);

  // NOTE The Tabs interface section is a fixed TreeWidget entry!
  m_cfgTabs = new ConfigTabsView(m_pageView);
  m_pageView->insertWidget(_pindex++, m_cfgTabs);

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

  connect(m_treeWidget, SIGNAL(sendConfigGroup(const QString &)),
          SLOT(openConfigGroup(const QString &)));
  connect(m_treeWidget, SIGNAL(sendPageIndex(int)), SLOT(setOpenPage(int)));

  connect(btn_save, SIGNAL(clicked()), this, SLOT(aboutToSave()));
  connect(btn_close, SIGNAL(clicked()), this, SLOT(aboutToClose()));
}

ConfigDialog::~ConfigDialog() {}

const QList<AntiquaCRM::TabsConfigWidget *> ConfigDialog::pages() {
  return m_pageView->findChildren<AntiquaCRM::TabsConfigWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

AntiquaCRM::TabsConfigWidget *ConfigDialog::page(int index) {
  AntiquaCRM::TabsConfigWidget *_w = nullptr;
  _w = qobject_cast<AntiquaCRM::TabsConfigWidget *>(m_pageView->widget(index));
  return _w;
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

bool ConfigDialog::loadTabPlugins() {
  QList<AntiquaCRM::TabsInterface *> _list = m_cfgTabs->viewableTabs();
  if (_list.size() > 0) {
    int _c = m_pageView->count();
    for (int i = 0; i < _list.size(); i++) {
      AntiquaCRM::TabsConfigWidget *_w = _list.at(i)->configWidget(m_pageView);
      if (_w != nullptr) {
        m_pageView->insertWidget(_c, _w);
        m_treeWidget->addTabPlugin(_c, _w->getMenuEntry());
        _c++;
      }
    }
    return true;
  }
  return false;
}

bool ConfigDialog::loadProviderPlugins() {
  // AntiquaCRM::ProvidersLoader _loader(this);
  // QList<AntiquaCRM::ProviderInterface *> _list = _loader.interfaces(this);
  int _c = m_pageView->count();
  m_pageView->insertWidget(_c++, new ConfigProvidersView(m_pageView));
  return true;
}

void ConfigDialog::loadConfigs() {
  QListIterator<AntiquaCRM::TabsConfigWidget *> it(pages());
  while (it.hasNext()) {
    it.next()->loadSectionConfig();
  }
}

void ConfigDialog::statusMessage(const QString &message) {
  m_statusbar->showMessage(message, 5000);
}

void ConfigDialog::openConfigGroup(const QString &name) {
  if (!name.startsWith("config_"))
    return;

  for (int i = 0; i < m_pageView->count(); i++) {
    if (m_pageView->widget(i)->objectName() == name) {
      setOpenPage(i);
      return;
    }
  }
}

void ConfigDialog::setOpenPage(int index) {
  AntiquaCRM::TabsConfigWidget *_page = page(index);
  if (_page == nullptr)
    return;

  QString _title(tr("Configuration"));
  _title.append(" (");
  _title.append(_page->getMenuEntry().value("title").toString());
  _title.append(") [*]");

  setWindowTitle(_title);

  m_pageView->setCurrentIndex(index);
}

void ConfigDialog::aboutToSave() {
  QListIterator<AntiquaCRM::TabsConfigWidget *> it(pages());
  while (it.hasNext()) {
    it.next()->saveSectionConfig();
  }
}

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

  if (!loadTabPlugins())
    return QDialog::Rejected;

  loadProviderPlugins();
  loadConfigs();
  setWindowModified(false);
  return QDialog::exec();
}
