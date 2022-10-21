// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquawindow.h"
#include "antiquamenubar.h"
#include "antiquastatusbar.h"
#include "antiquatabwidget.h"

#include <QMessageBox>

AntiquaWindow::AntiquaWindow(QWidget *parent) : QMainWindow{parent} {
  setMinimumSize(QSize(750, 550));
  setWindowTitle(QString(ANTIQUACRM_WINDOW_TITLE) + " [*]");
  setWindowIcon(QIcon(":icons/antiqua.png"));

  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->setObjectName("window_settings");

  m_menuBar = new AntiquaMenuBar(this);
  setMenuBar(m_menuBar);

  m_centralWidget = new AntiquaTabWidget(this);
  m_centralWidget->setEnabled(false);
  setCentralWidget(m_centralWidget);

  m_statusBar = new AntiquaStatusBar(this);
  setStatusBar(m_statusBar);

  connect(m_menuBar, SIGNAL(sendApplQuit()), SLOT(closeWindow()));
  connect(m_menuBar, SIGNAL(sendOpenTabViews(const QString &)), m_centralWidget,
          SLOT(addViewsTab(const QString &)));
  connect(m_menuBar, SIGNAL(sendToggleFullscreen()),
          SLOT(setToggleFullScreen()));
}

void AntiquaWindow::hideEvent(QHideEvent *event) {
  if (isVisible() && event->isAccepted()) {
    m_cfg->setValue("window/geometry", saveGeometry());

    if (isFullScreen()) // Keine Vollansicht Speichern!
      setWindowState(windowState() & ~Qt::WindowFullScreen);

    m_cfg->setValue("window/windowState", saveState());
  }
  QMainWindow::hideEvent(event);
}

void AntiquaWindow::closeEvent(QCloseEvent *event) {
  // Beendet wird über das SIGNAL sendApplQuit() zur parent Klasse.
  QHideEvent hide;
  hide.setAccepted(true);
  hideEvent(&hide);
}

void AntiquaWindow::closeWindow() {
  if (isWindowModified()) {
    QString t = tr("Save request");
    QStringList b("<b>You have unsaved changes.</b>");
    b << tr("<p>Do you really want to close the application?</p>");
    int ret = QMessageBox::question(this, t, b.join("<br/>"));
    if (ret == QMessageBox::No) {
      return;
    }
  }

  QByteArray ba = saveGeometry();
  if (!ba.isNull())
    m_cfg->setValue("window/geometry", ba);

  ba.clear();
  emit sendApplQuit();
}

void AntiquaWindow::setToggleWindow() { (isVisible()) ? hide() : show(); }

void AntiquaWindow::setToggleFullScreen() {
  if (isFullScreen()) {
    setWindowState(windowState() & ~Qt::WindowFullScreen);
  } else {
    setWindowState(windowState() ^ Qt::WindowFullScreen);
  }
}

void AntiquaWindow::openWindow() {
  if (m_cfg->contains("window/geometry"))
    restoreGeometry(m_cfg->value("window/geometry").toByteArray());

  show();

  if (m_cfg->contains("window/windowState"))
    restoreState(m_cfg->value("window/windowState").toByteArray());

  if (m_centralWidget->loadDefaultTabs())
    m_centralWidget->setEnabled(true);
}
