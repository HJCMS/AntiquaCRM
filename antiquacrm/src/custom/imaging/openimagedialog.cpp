// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "openimagedialog.h"
#include "antiqua_global.h"
#include "imageview.h"
#include "listview.h"
#include "myicontheme.h"
#include "toolbar.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QImageReader>
#include <QList>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

OpenImageDialog::OpenImageDialog(QWidget *parent) : QDialog(parent) {
  setObjectName("OpenImageDialogMain");
  setWindowTitle(tr("Open Image from Disk"));
  setShortcutEnabled(false);
  setSizeGripEnabled(true);
  setMinimumSize(QSize(500, 400));

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new ToolBar(this);
  layout->addWidget(m_toolBar);

  m_splitter = new QSplitter(this);
  m_splitter->setOrientation(Qt::Horizontal);
  m_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_splitter->setMinimumHeight(250);
  layout->addWidget(m_splitter);

  m_thumbsView = new ImageView(this);
  m_thumbsView->setMinimumWidth(125);
  m_splitter->insertWidget(0, m_thumbsView);
  m_splitter->setCollapsible(0, false);
  m_splitter->setStretchFactor(0, 35);

  m_listView = new ListView(this);
  m_listView->setMinimumWidth(250);
  m_listView->setToolTip(
      tr("Onclick for preview and double click to set image for open."));
  m_splitter->insertWidget(1, m_listView);
  m_splitter->setCollapsible(1, false);
  m_splitter->setStretchFactor(1, 65);

  m_lineEdit = new QLineEdit(this);
  layout->addWidget(m_lineEdit);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);
  layout->addWidget(m_buttonBox);

  QPushButton *m_save =
      m_buttonBox->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
  m_save->setIcon(myIcon("filesave"));
  m_save->setEnabled(false);
  m_save->setObjectName("OkButtonDialog");

  QPushButton *m_close =
      m_buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);
  m_close->setIcon(myIcon("button_cancel"));
  m_close->setObjectName("CloseButtonDialog");

  setLayout(layout);

  connect(m_listView, SIGNAL(s_previewImage(const QString &)), m_thumbsView,
          SLOT(setImage(const QString &)));

  connect(m_listView, SIGNAL(s_selectionChanged(const QString &)), this,
          SLOT(isValid(const QString &)));

  connect(this, SIGNAL(imageChanged(bool)), m_save, SLOT(setEnabled(bool)));

  connect(m_toolBar, SIGNAL(goUp()), m_listView, SLOT(goUpward()));
  connect(m_toolBar, SIGNAL(goTo(const QString &)), m_listView,
          SLOT(goTo(const QString &)));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void OpenImageDialog::isValid(const QString &path) {
  QFileInfo info(path);
  if (info.isReadable()) {
    m_lineEdit->setText(info.absoluteFilePath());
    emit imageChanged(true);
    return;
  }
  emit imageChanged(false);
}

void OpenImageDialog::setStart(const QString &path) {
  if (!path.isEmpty())
    m_listView->goTo(path);
}

const QImage OpenImageDialog::image() {
  QString path = m_lineEdit->text();
  if (!path.isEmpty()) {
    QImageReader reader(path);
    reader.setAutoTransform(true);
    return reader.read();
  }
  return QImage();
}

const QFileInfo OpenImageDialog::file() {
  QFileInfo info(m_lineEdit->text());
  return info;
}

OpenImageDialog::~OpenImageDialog() {}
