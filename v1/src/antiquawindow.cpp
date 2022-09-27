// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquawindow.h"
#include "antiquamenubar.h"
#include "antiquastatusbar.h"
#include "antiquatabwidget.h"

AntiquaWindow::AntiquaWindow(QWidget *parent) : QMainWindow{parent} {
  setMinimumSize(QSize(500, 450));
  setWindowTitle("Antiqua CRM [*]");
  setWindowIcon(QIcon(":icons/antiqua.png"));

  m_menuBar = new AntiquaMenuBar(this);
  setMenuBar(m_menuBar);

  m_centralWidget = new AntiquaTabWidget(this);
  setCentralWidget(m_centralWidget);

  m_statusBar = new AntiquaStatusBar(this);
  setStatusBar(m_statusBar);
}

void AntiquaWindow::closeEvent(QCloseEvent *event) {
  if (isFullScreen()) // Keine Vollansicht Speichern!
    setWindowState(windowState() & ~Qt::WindowFullScreen);
  // TODO Save operations

  QMainWindow::closeEvent(event);
}
