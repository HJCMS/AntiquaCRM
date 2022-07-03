// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagelocation.h"
#include "myicontheme.h"

#include <QRegularExpressionMatch>
#include <QStatusTipEvent>
#include <QtCore>
#include <QtWidgets>

#ifndef SQL_FIELD_PATTERN
#define SQL_FIELD_PATTERN "^([\\w\\d]+)([\\w\\d\\s\\.\\/\\`\\-]+)$"
#endif

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget{parent}, pattern(SQL_FIELD_PATTERN,
                               QRegularExpression::UseUnicodePropertiesOption) {
  QGridLayout *layout = new QGridLayout(this);

  layout->addWidget(new QLabel(tr("Storage ID")), 0, 0, 1, 1, Qt::AlignRight);
  sl_id = new QSpinBox(this);
  sl_id->setRange(0, 9999);
  sl_id->setReadOnly(true);
  layout->addWidget(sl_id, 0, 1, 1, 1, Qt::AlignLeft);

  QString inf = tr("Empty data fields not accepted and reject this operation.");
  layout->addWidget(new QLabel(inf, this), 1, 0, 1, 2, Qt::AlignLeft);

  layout->addWidget(new QLabel(tr("Storage")), 2, 0, 1, 1, Qt::AlignRight);
  sl_storage = new QLineEdit(this);
  layout->addWidget(sl_storage, 2, 1, 1, 1, Qt::AlignLeft);

  layout->addWidget(new QLabel(tr("Identifier")), 3, 0, 1, 1, Qt::AlignRight);
  sl_identifier = new QLineEdit(this);
  layout->addWidget(sl_identifier, 3, 1, 1, 1);

  layout->addWidget(new QLabel(tr("Location")), 4, 0, 1, 1, Qt::AlignRight);
  sl_location = new QLineEdit(this);
  layout->addWidget(sl_location, 4, 1, 1, 1);

  setLayout(layout);
}

bool EditorWidget::check(QLineEdit *w) {
  QString str = w->text();
  str = str.trimmed();
  QRegularExpressionMatch m = pattern.match(str);
  if (m.hasMatch()) {
    return true;
  }
  w->setFocus();
  return false;
}

void EditorWidget::setValue(const StorageTable::RowValues &items) {
  sl_id->setValue(items.sl_id);
  sl_storage->setText(items.sl_storage);
  sl_identifier->setText(items.sl_identifier);
  sl_location->setText(items.sl_location);
}

void EditorWidget::clear() {
  sl_id->setValue(0);
  sl_storage->clear();
  sl_identifier->clear();
  sl_location->clear();
  sl_storage->setFocus();
}

const QString EditorWidget::sqlQuery() {
  QString sql;
  if (check(sl_storage) && check(sl_identifier) && check(sl_location)) {
    int id = sl_id->value();
    if (id < 1) {
      sql.append("INSERT INTO ref_storage_location ");
      sql.append("(sl_storage,sl_identifier,sl_location)");
      sql.append(" VALUES ('");
      sql.append(sl_storage->text().trimmed());
      sql.append("','");
      sql.append(sl_identifier->text().trimmed());
      sql.append("','");
      sql.append(sl_location->text().trimmed());
      sql.append("') RETURNING sl_id;");
    } else {
      sql.append("UPDATE ref_storage_location SET ");
      sql.append("sl_storage='");
      sql.append(sl_storage->text().trimmed());
      sql.append("',sl_identifier='");
      sql.append(sl_identifier->text().trimmed());
      sql.append("',sl_location='");
      sql.append(sl_location->text().trimmed());
      sql.append("' WHERE sl_id=" + QString::number(id) + ";");
    }
  }
  return sql;
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

  connect(m_table, SIGNAL(itemChanged(const StorageTable::RowValues &)),
          m_editorWidget, SLOT(setValue(const StorageTable::RowValues &)));
  connect(m_table, SIGNAL(queryMessages(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
  connect(ac_add, SIGNAL(triggered()), m_editorWidget, SLOT(clear()));
  connect(ac_search, SIGNAL(triggered()), this, SLOT(searchClicked()));
  connect(m_search, SIGNAL(textChanged(const QString &)), m_table,
          SLOT(findColumn(const QString &)));
  connect(m_btnBox, SIGNAL(accepted()), this, SLOT(saveClicked()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(accept()));
}

void StorageLocation::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
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

void StorageLocation::searchClicked() {
  QString str = m_search->text().trimmed();
  if (!str.isEmpty())
    m_table->findColumn(str);
}

void StorageLocation::saveClicked() {
  QString sql = m_editorWidget->sqlQuery();
  if (sql.isEmpty()) {
    m_statusBar->showMessage(tr("invalid input"), 1000);
    return;
  }
  m_table->sqlQuery(sql);
}
