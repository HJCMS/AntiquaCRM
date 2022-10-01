// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquawindow.h"
#include "antiquamenubar.h"
#include "antiquastatusbar.h"
#include "antiquatabwidget.h"

#include <QMessageBox>

AntiquaWindow::AntiquaWindow(QWidget *parent) : QMainWindow{parent} {
  setMinimumSize(QSize(500, 450));
  setWindowTitle("Antiqua CRM [*]");
  setWindowIcon(QIcon(":icons/antiqua.png"));

  m_menuBar = new AntiquaMenuBar(this);
  setMenuBar(m_menuBar);

  m_centralWidget = new AntiquaTabWidget(this);
  m_centralWidget->setEnabled(false);
  setCentralWidget(m_centralWidget);

  m_statusBar = new AntiquaStatusBar(this);
  setStatusBar(m_statusBar);

  connect(m_menuBar, SIGNAL(sendApplicationQuit()), this, SLOT(closeWindow()));
}

void AntiquaWindow::closeEvent(QCloseEvent *event) {
  if (isFullScreen()) // Keine Vollansicht Speichern!
    setWindowState(windowState() & ~Qt::WindowFullScreen);
  // TODO Save operations

  QMainWindow::closeEvent(event);
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
  emit sendApplicationQuit();
}

void AntiquaWindow::openWindow() {
  show();
  activateWindow();
  m_centralWidget->setEnabled(true);
  m_centralWidget->loadDefaultTabs();
}
