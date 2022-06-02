// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagelocation.h"
#include "myicontheme.h"

#include <QStatusTipEvent>
#include <QtCore>
#include <QtWidgets>

EditorWidget::EditorWidget(QWidget *parent) : QWidget{parent} {
  QGridLayout *layout = new QGridLayout(this);

  layout->addWidget(new QLabel(tr("Storage ID")), 0, 0, 1, 1, Qt::AlignRight);
  sl_id = new QSpinBox(this);
  sl_id->setReadOnly(true);
  layout->addWidget(sl_id, 0, 1, 1, 1, Qt::AlignLeft);

  layout->addWidget(new QLabel(tr("Storage")), 1, 0, 1, 1, Qt::AlignRight);
  sl_storage = new QLineEdit(this);
  layout->addWidget(sl_storage, 1, 1, 1, 1, Qt::AlignLeft);

  layout->addWidget(new QLabel(tr("Identifier")), 2, 0, 1, 1, Qt::AlignRight);
  sl_identifier = new QLineEdit(this);
  layout->addWidget(sl_identifier, 2, 1, 1, 1);

  layout->addWidget(new QLabel(tr("Location")), 3, 0, 1, 1, Qt::AlignRight);
  sl_location = new QLineEdit(this);
  layout->addWidget(sl_location, 3, 1, 1, 1);

  setLayout(layout);
}

void EditorWidget::setValue(const StorageTable::Item &item) {
  sl_id->setValue(item.sl_id);
  sl_storage->setText(item.sl_storage);
  sl_identifier->setText(item.sl_identifier);
  sl_location->setText(item.sl_location);
}

StorageLocation::StorageLocation(QWidget *parent) : QDialog{parent} {
  setObjectName("storage_location");
  setMinimumSize(700, 400);
  setContentsMargins(0, 0, 0, 0);
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new QToolBar(this);
  m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  ac_add = m_toolBar->addAction(myIcon("db_add"), tr("new entry"));
  ac_add->setStatusTip(tr("insert a new storage location"));
  ac_add->setEnabled(false);
  m_toolBar->addSeparator();
  m_search = new QLineEdit(this);
  m_search->setObjectName("search");
  m_toolBar->addWidget(m_search);
  ac_search = m_toolBar->addAction(myIcon("search"), tr("search"));
  ac_search->setStatusTip(tr("search in table"));
  m_toolBar->addSeparator();
  layout->addWidget(m_toolBar);

  m_table = new StorageTable(this);
  layout->addWidget(m_table);

  m_editorWidget = new EditorWidget(this);
  layout->addWidget(m_editorWidget);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Save |
                               QDialogButtonBox::Close);

  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_table, SIGNAL(itemChanged(const StorageTable::Item &)),
          m_editorWidget, SLOT(setValue(const StorageTable::Item &)));
  connect(m_search, SIGNAL(textChanged(const QString &)), m_table,
          SLOT(findColumn(const QString &)));
  connect(m_btnBox, SIGNAL(accepted()), this, SLOT(saveClicked()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(accept()));
}

void StorageLocation::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return) {
    return;
  }
  QDialog::keyPressEvent(e);
}

bool StorageLocation::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = reinterpret_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void StorageLocation::saveClicked() { qDebug() << Q_FUNC_INFO; }
